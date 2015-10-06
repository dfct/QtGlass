#include "qtglass.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QtGlass w;

	//Remove the default tan system background from the window
	w.setAttribute(Qt::WA_TranslucentBackground);
	
	//Set an initial window size
	w.setMinimumSize(800, 600);

	//Extend the Aero glass frame into the window 
	QtWin::extendFrameIntoClientArea(&w, -1, -1, -1, -1);

	//Draw the window
	w.show();

	return a.exec();
}


QtGlass::QtGlass(QWidget *parent)
	: QMainWindow(parent)
{

}

QtGlass::~QtGlass()
{

}

//Capture window messaging events
bool QtGlass::nativeEvent(const QByteArray& eventType, void* message, long* result)
{
	//Cast the window message to a type we can work with
	MSG *msg = static_cast<MSG *>(message);

	//Check if DWM handled the message; if it did, it wasn't in our client area and we can ignore it 
	if (DwmDefWindowProc(msg->hwnd, msg->message, msg->wParam, msg->lParam, (LRESULT*)result))
		return true;


	//Get the window rectangle 
	RECT winrect;
	GetWindowRect(msg->hwnd, &winrect);

	//Condition based on the message type
	switch (msg->message)
	{
		//On show window, update the frame
		case WM_SHOWWINDOW:
		{
			SetWindowPos(
				msg->hwnd,
				NULL,
				winrect.left, 
				winrect.top,
				winrect.right - winrect.left, 
				winrect.bottom - winrect.top,
				SWP_FRAMECHANGED
				);
			return false;
		}
		//On calculate size, remove the frame
		case WM_NCCALCSIZE:
		{
			if (msg->wParam)
			{
				//Set the result to 0 to remove the window frame and caption items extending the client area into the frame
				//https://msdn.microsoft.com/en-us/library/windows/desktop/ms632634.aspx
				*result = 0;

				//Return true to indicate we've acted on this event
				return true;
			}
		}
		//Allow dragging the window
		case WM_NCHITTEST:
		{
			//Get the size of the title bar
			const int titleSize = GetSystemMetrics(SM_CYCAPTION);

			//Get Y mouse coordinate
			long y = GET_Y_LPARAM(msg->lParam);

			//If the mouse is within the title bar area, allow dragging the window
			if (y >= winrect.top && y < winrect.top + titleSize)
			{
				*result = HTCAPTION;
				return true;
			}				

			/*To allow dragging from within the entire window if the mouse event was not processed by a child object, uncomment this*/

			//QWidget *action = QApplication::widgetAt(QCursor::pos());
			//if (action == this)
			//{
			//	*result = HTCAPTION;
			//	return true;
			//}


			/* To allow resizing the window, uncomment this */

			////Get X and Y mouse coordinates
			//long x = GET_X_LPARAM(msg->lParam);
			//long y = GET_Y_LPARAM(msg->lParam);
			//
			////Set the border size to show the resize cursor for
			//const int border_width = 8; /* in pixels */

			////left border
			//if (x >= winrect.left && x < winrect.left + border_width)
			//{
			//	*result = HTLEFT;
			//	return true;
			//}
			////right border
			//if (x < winrect.right && x >= winrect.right - border_width)
			//{
			//	*result = HTRIGHT;
			//	return true;
			//}
			////bottom border
			//if (y < winrect.bottom && y >= winrect.bottom - border_width)
			//{
			//	*result = HTBOTTOM;
			//	return true;
			//}
			////top border
			//if (y >= winrect.top && y < winrect.top + border_width)
			//{
			//	*result = HTTOP;
			//	return true;
			//}
			////bottom left corner
			//if (x >= winrect.left && x < winrect.left + border_width &&
			//	y < winrect.bottom && y >= winrect.bottom - border_width)
			//{
			//	*result = HTBOTTOMLEFT;
			//	return true;
			//}
			////bottom right corner
			//if (x < winrect.right && x >= winrect.right - border_width &&
			//	y < winrect.bottom && y >= winrect.bottom - border_width)
			//{
			//	*result = HTBOTTOMRIGHT;
			//	return true;
			//}
			////top left corner
			//if (x >= winrect.left && x < winrect.left + border_width &&
			//	y >= winrect.top && y < winrect.top + border_width)
			//{
			//	*result = HTTOPLEFT;
			//	return true;
			//}
			////top right corner
			//if (x < winrect.right && x >= winrect.right - border_width &&
			//	y >= winrect.top && y < winrect.top + border_width)
			//{
			//	*result = HTTOPRIGHT;
			//	return true;
			//}

		}
	}

	//Pass the event on if it wasn't for us to react to
	return QWidget::nativeEvent(eventType, message, result);
}