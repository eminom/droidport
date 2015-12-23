
package com.haibai.libdroidport;

import android.app.Activity;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.content.SharedPreferences;
import android.content.pm.ApplicationInfo;
import android.content.res.AssetManager;
import android.net.Uri;
import android.os.Build;
import android.os.IBinder;
import android.os.Vibrator;
import android.preference.PreferenceManager.OnActivityResultListener;
import android.util.DisplayMetrics;
import android.view.Display;
import android.view.WindowManager;

import java.io.UnsupportedEncodingException;
import java.util.LinkedHashSet;
import java.util.Locale;
import java.util.Map;
import java.util.Set;

public class DroidHelper {

    public static final String Tag = DroidHelper.class.getSimpleName();
    private static Activity sActivity = null;
    private static boolean sInited = false;
    private static String sPackageName;
    private static String sFileDirectory;

    public static void init(final Activity activity) {
        sActivity = activity;
        if (!sInited) {
            sInited = true;
            final ApplicationInfo applicationInfo = activity.getApplicationInfo();
            DroidHelper.sPackageName = applicationInfo.packageName;
            DroidHelper.sFileDirectory = activity.getFilesDir().getAbsolutePath();

            //Cocos2dxHelper.nativeSetContext((Context)activity);
            DroidHelper.nativeSetContext(activity);

            //Enhance API modification begin
            //Intent serviceIntent = new Intent(IGameTuningService.class.getName());
            //serviceIntent.setPackage("com.enhance.gameservice");
            //boolean suc = activity.getApplicationContext().bindService(serviceIntent, connection, Context.BIND_AUTO_CREATE);
            //Enhance API modification end
        }
    }

    private static native void nativeSetContext(final Context context);
}