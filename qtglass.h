#ifndef QTGLASS_H
#define QTGLASS_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QApplication>

#include <QtWinExtras/QtWin>				//Qt Windows Extras for extendFrameIntoClientArea function
#pragma comment(lib, "Qt5WinExtras.lib")	//Requires a library

#include "Dwmapi.h"							//DWM Api header for the DwmDefWindowProc function
#pragma comment(lib, "dwmapi.lib")			//Requires a library

#include "Windowsx.h"						//Macros header for GET_X_LPARAM and GET_Y_LPARAM macros


class QtGlass : public QMainWindow
{
	Q_OBJECT

public:
	QtGlass(QWidget *parent = 0);
	~QtGlass();

private:
	//Capture window messaging events
	bool nativeEvent(const QByteArray& eventType, void* message, long* result) override;
};

#endif // QTGLASS_H
