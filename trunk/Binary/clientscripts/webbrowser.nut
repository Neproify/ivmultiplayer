local screen = guiGetScreenSize();
local Width = screen[0]/2;
local Height = screen[1]/2;

local webWindow = GUIWindow();
local webView = GUIWebView(Width.tointeger(), Height.tointeger(), "http://iv-m.com");
local webURL = GUIEditBox();
local webGo = GUIButton();
local webGoname = webGo.getName();



local isActive = false;

function onScriptInit( )
{
    webWindow.setSize(Width + 10, Height + 100, false);
    webWindow.setPosition(screen[0] - (Width*1.5), screen[1] - (Height*1.5), false);

    webView.setParent(webWindow.getName());
    webView.setPosition(0.0, 75.0, false);
    webView.setURI("http://iv-m.com");
    webView.registerJavaScriptMethod("TestMethod");

    webURL.setSize(Width - 70.0, 60.0, false);
    webURL.setParent(webWindow.getName());
    webURL.setPosition(0.0, 5.0, false);

    webGo.setSize(65.0, 60.0, false);
    webGo.setParent(webWindow.getName());
    webGo.setPosition(Width - 65.0, 5.0, false);
    webGo.setText("GO!");

    webView.setVisible(true);
    webWindow.setVisible(false);
    return 1;
}
addEvent("scriptInit", onScriptInit);

addEvent("keyPress", function (key, state) 
{
    if(key == "b") 
    {
        if(state == "down")
        {
            if(isActive)
            {
                webWindow.setVisible(false);
                guiToggleCursor(false);
                isActive = false;
            }
            else
            {
                webWindow.setVisible(true);
                guiToggleCursor(true);
                isActive = true;
	webView.evaluateJavaScript("IVMP.TestMethod(15, 255.1, \"Opened\");");
            }
        }
    }
});

function onButtonClick(btnName, bState)
{
    if(btnName == webGo.getName())
    {
          webView.setURI(webURL.getText());
    }
}
addEvent("buttonClick", onButtonClick);

function onWebkitLoadUpdate(webViewName, completed, contentLength, lastChangeTime, title, statusCode, uri)
{
	//addChatMessage("Loaded URI: " + uri + " With title: " + title);
	if(webViewName == webView.getName())
	{
		webWindow.setText(title);
	}
}
addEvent("webkitLoadUpdate", onWebkitLoadUpdate);

function onWebkitJSMethodInvoked(webViewName, methodName, args)
{
	addChatMessage("Browser " + args[2]);
}
addEvent("webkitJSMethodInvoked", onWebkitJSMethodInvoked);