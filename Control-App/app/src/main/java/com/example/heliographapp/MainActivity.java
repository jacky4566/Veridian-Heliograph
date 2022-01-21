package com.example.heliographapp;

import androidx.appcompat.app.AppCompatActivity;
import android.Manifest;
import android.os.Bundle;

import com.google.android.gms.maps.GoogleMap;
import com.google.android.gms.maps.OnMapReadyCallback;
import com.google.android.gms.maps.SupportMapFragment;
import com.google.android.gms.maps.UiSettings;
import com.google.android.gms.maps.model.LatLng;
import com.google.android.gms.maps.model.MarkerOptions;

import java.util.List;

import pub.devrel.easypermissions.AppSettingsDialog;
import pub.devrel.easypermissions.EasyPermissions;
import pub.devrel.easypermissions.PermissionRequest;

public class MainActivity extends AppCompatActivity implements OnMapReadyCallback, EasyPermissions.PermissionCallbacks {

    private String[] perms = {Manifest.permission.ACCESS_COARSE_LOCATION, Manifest.permission.ACCESS_FINE_LOCATION};
    private boolean hasPermissions = false;

    private GoogleMap mMap;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main_view);

        if (EasyPermissions.hasPermissions(this, Manifest.permission.ACCESS_FINE_LOCATION, Manifest.permission.ACCESS_COARSE_LOCATION)) {
            hasPermissions = true;
        }else{
            EasyPermissions.requestPermissions(this, getString(R.string.rationale_location), 1, perms);
        }

        // Get a handle to the fragment and register the callback.
        SupportMapFragment mapFragment = (SupportMapFragment) getSupportFragmentManager().findFragmentById(R.id.map);
        mapFragment.getMapAsync(this);
    }

    @Override
    public void onMapReady(GoogleMap mMap) {
        this.mMap = mMap;
        UiSettings uiSettings = mMap.getUiSettings();
        uiSettings.setRotateGesturesEnabled(true);
        uiSettings.setTiltGesturesEnabled(true);
        uiSettings.setMyLocationButtonEnabled(true);
        uiSettings.setMapToolbarEnabled(true);
        uiSettings.setCompassEnabled(true);
        mMap.setMapType(GoogleMap.MAP_TYPE_HYBRID);
        if (hasPermissions) {
            mMap.setMyLocationEnabled(true);
        }
    }


    @Override
    public void onPermissionsGranted(int requestCode, List<String> list) {
        if (mMap != null) {
            mMap.setMyLocationEnabled(true);
        }
    }

    @Override
    public void onPermissionsDenied(int requestCode, List<String> list) {

    }
}