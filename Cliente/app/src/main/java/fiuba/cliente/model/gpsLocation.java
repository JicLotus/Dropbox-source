package fiuba.cliente.model;

import android.content.Context;
import android.location.Location;
import android.location.LocationManager;

/**
 * Created by lotus on 2/11/15.
 */

public class gpsLocation {
    double latitude;
    double longitude;

    protected LocationManager locationManager;
    Location location;
    Context context;

    public gpsLocation(Context contextParam)
    {
        context = contextParam;
        location = getLocation();
    }

    public Location getLocation() {
        try {
            locationManager = (LocationManager) context.getSystemService(Context.LOCATION_SERVICE);

            boolean  isGPSEnabled = locationManager.isProviderEnabled(LocationManager.GPS_PROVIDER);

            boolean isNetworkEnabled = locationManager.isProviderEnabled(LocationManager.NETWORK_PROVIDER);



            if (!isGPSEnabled && !isNetworkEnabled) {
                // no network provider is enabled
            }
            else {
                if (locationManager != null) {
                    try{location = locationManager.getLastKnownLocation(LocationManager.NETWORK_PROVIDER);}catch(Exception e){}
                    if (location != null) {
                        latitude = location.getLatitude();
                        longitude = location.getLongitude();
                    }
                }
            }


            if (isGPSEnabled) {
                if (location == null) {
                    if (locationManager != null) {
                        try{location = locationManager.getLastKnownLocation(LocationManager.GPS_PROVIDER);}catch(Exception e){}
                        if (location != null) {
                            latitude = location.getLatitude();
                            longitude = location.getLongitude();
                        }
                    }
                }
            }

        } catch (Exception e) {
            e.printStackTrace();
        }

        return location;
    }


    public double getLatitude()
    {
        try {
            return location.getLatitude();
        } catch (Exception e) {
            return 0.0;
        }
    }

    public double getLongitude()
    {
        try {
            return location.getLongitude();
        } catch (Exception e) {
            return 0.0;
        }
    }

}
