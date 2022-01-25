package com.example.heliographapp;

import androidx.appcompat.app.AppCompatActivity;
import android.Manifest;
import android.annotation.SuppressLint;
import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.IBinder;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.Toast;

import com.google.android.gms.maps.GoogleMap;
import com.google.android.gms.maps.OnMapReadyCallback;
import com.google.android.gms.maps.SupportMapFragment;
import com.google.android.gms.maps.UiSettings;

import java.util.ArrayList;
import java.util.List;
import java.util.Set;

import pub.devrel.easypermissions.EasyPermissions;

public class MainActivity extends AppCompatActivity implements
        OnMapReadyCallback,
        EasyPermissions.PermissionCallbacks,
        View.OnClickListener,
        AdapterView.OnItemSelectedListener
{

    //Permissions
    private String[] perms = {Manifest.permission.ACCESS_COARSE_LOCATION,
            Manifest.permission.ACCESS_FINE_LOCATION,
            Manifest.permission.BLUETOOTH,
            Manifest.permission.BLUETOOTH_CONNECT,
            Manifest.permission.BLUETOOTH_SCAN
    };
    private boolean hasPermissions = false;

    //GUI
    private Button btnUpdate;
    private GoogleMap mMap;
    private Spinner spinner;

    //BLE
    private BluetoothAdapter bluetoothAdapter;
    private ArrayList<String> foundDevices = new ArrayList<>();
    private ArrayAdapter aa;
    private BluetoothLeService bluetoothService;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main_view);

        //GUI
        btnUpdate = (Button) findViewById(R.id.buttonUpdate) ;
        btnUpdate.setOnClickListener(this);
        spinner = (Spinner) findViewById(R.id.deviceSpinner);
        spinner.setOnItemSelectedListener(this);

        //Permissions
        if (EasyPermissions.hasPermissions(this, perms)) {
            hasPermissions = true;
        }else{
            EasyPermissions.requestPermissions(this, getString(R.string.rationale_location), 1, perms);
        }

        //BLE
        bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        if (bluetoothAdapter == null) {
            // Device doesn't support Bluetooth
            Toast.makeText(this, "error: bluetooth not supported", Toast.LENGTH_LONG).show();
        }
        IntentFilter filter = new IntentFilter(BluetoothDevice.ACTION_FOUND);
        registerReceiver(receiver, filter);
        startScanner();
        aa = new ArrayAdapter<String>(this, android.R.layout.simple_list_item_1, foundDevices);
        spinner.setAdapter(aa);
        Intent gattServiceIntent = new Intent(this, BluetoothLeService.class);
        bindService(gattServiceIntent, serviceConnection, Context.BIND_AUTO_CREATE);

        // Get a handle to the fragment and register the callback.
        SupportMapFragment mapFragment = (SupportMapFragment) getSupportFragmentManager().findFragmentById(R.id.map);
        mapFragment.getMapAsync(this);
    }

    private ServiceConnection serviceConnection = new ServiceConnection() {
        @Override
        public void onServiceConnected(ComponentName name, IBinder service) {
            bluetoothService = ((BluetoothLeService.LocalBinder) service).getService();
            if (bluetoothService != null) {
                // call functions on service to check connection and connect to devices
            }
        }

        @Override
        public void onServiceDisconnected(ComponentName name) {
            bluetoothService = null;
        }
    };

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

    public void onClick(View v){
        if (v == btnUpdate){
            Toast.makeText(this, "btnUpdate", Toast.LENGTH_LONG).show();
        }
    }

    public void startScanner(){
        final int REQUEST_ENABLE_BT = 1;
        if (!bluetoothAdapter.isEnabled()) {
            Intent enableBluetoothIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableBluetoothIntent, REQUEST_ENABLE_BT);
        }
        if (bluetoothAdapter.isDiscovering()) {
            bluetoothAdapter.cancelDiscovery();
        }
        bluetoothAdapter.startDiscovery();
    }

    // Create a BroadcastReceiver for ACTION_FOUND.
    private final BroadcastReceiver receiver = new BroadcastReceiver() {
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();
            if (BluetoothDevice.ACTION_FOUND.equals(action)) {
                // Discovery has found a device. Get the BluetoothDevice
                // object and its info from the Intent.
                BluetoothDevice device = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
                String deviceCommon = device.getAddress();
                Log.i("BLE", deviceCommon);
                foundDevices.add(deviceCommon);
                aa.notifyDataSetChanged();
            }
        }
    };

    public void onItemSelected(AdapterView<?> parent, View view,
                               int pos, long id) {
        Toast.makeText(getApplicationContext(),parent.getItemAtPosition(pos).toString() , Toast.LENGTH_SHORT).show();
    }

    public void onNothingSelected(AdapterView<?> parent) {
        // Another interface callback
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

    @Override
    protected void onDestroy() {
        super.onDestroy();
        unregisterReceiver(receiver);
        if (bluetoothAdapter.isDiscovering()) {
            bluetoothAdapter.cancelDiscovery();
        }
    }
}