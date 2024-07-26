
class IOSNativeInterface
{

    showAR(url, settings, account)
    {
        window.webkit.messageHandlers.WebViewCtrl.postMessage({command: "SHOW_AR", url: url, settings: settings, account: account});
    }

    getAppInstanceId() {
        return window.DIMX_APP_INSTANCE_ID;
    }
    
    setWebAppHost(value) {
        window.webkit.messageHandlers.WebViewCtrl.postMessage({command: "SET_WEB_APP_HOST", value: value});
    }
    
    requestTrackingStatus(dimension) {
        window.webkit.messageHandlers.WebViewCtrl.postMessage({command: "REQUEST_TRACKING_STATUS", dimension: dimension});
    }
    
    requestGeolocationUpdate() {
        window.webkit.messageHandlers.WebViewCtrl.postMessage({command: "REQUEST_GEOLOCATION_UPDATE"});
    }
} // class IOSNativeInterface

window.Native = new IOSNativeInterface()
