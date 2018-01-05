package com.example.sucharitha.actualapp;

import android.app.Activity;
import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.widget.Toast;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.UUID;

public class MainActivity extends AppCompatActivity {

    Button b1, b2, b3, b4;
    BluetoothDevice device;
    BluetoothAdapter BA;
    ConnectedThread connectedThread;
    //private final BluetoothDevice mmDevice;
    public static final String TAG="MainActivity";
    private static final int REQUEST_BT_CONNECTION = 1;
    private static final int REQUEST_DISCOVERABLE = 2;
    private static final int  MESSAGE_READ =1;
    private static final int  SUCCESS_CONNECT =0;
    int connectsuccess=0;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Log.v(TAG,"in onCreate)");
        b1=(Button) findViewById(R.id.EnableBT);
        b2=(Button) findViewById(R.id.TurnOffBT);
        b3=(Button) findViewById(R.id.Send);
       // b4=(Button) findViewById(R.id.Receive);
        BA=BluetoothAdapter.getDefaultAdapter();
    }

    public void btenable(View v) {
        Log.v(TAG,"in btenable(View V)");
        if (!BA.isEnabled()) {
            turnonBT();
           // makeDeviceDiscoverable();

        } else if(BA.isEnabled()) {
           // makeDeviceDiscoverable();
           connectDevice();
            Toast.makeText(getApplicationContext(), "Already on", Toast.LENGTH_LONG).show();
        }else{
            Toast.makeText(getApplicationContext(),"No bluetooth support on device",Toast.LENGTH_SHORT).show();
        }

    }

    private void makeDeviceDiscoverable() {
        Log.v(TAG,"in discoverable method");
        Intent getVisible = new Intent(BluetoothAdapter.ACTION_REQUEST_DISCOVERABLE);
        startActivityForResult(getVisible, REQUEST_DISCOVERABLE);
    }

    public void turnonBT() {
        Intent turnOn = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
        startActivityForResult(turnOn, REQUEST_BT_CONNECTION);
        Toast.makeText(getApplicationContext(), "Turned on", Toast.LENGTH_LONG).show();
    }

    public void btoff(View v)
    {
        Log.v(TAG,"in off(View V)");
        BA.disable();
        Toast.makeText(getApplicationContext(), "Turned off", Toast.LENGTH_LONG).show();
    }
    public void callConnectedThread(BluetoothSocket socket)
    {
        Log.d(TAG,"Connectsuccess is 1");
        connectedThread = new ConnectedThread(socket);
        connectedThread.start();
    }
    public void send(View v)
    {
        String str="Connected\n";
        Log.d(TAG,"Trying to Wrte ");
        if(connectedThread!=null)
        {
            connectedThread.write(str.getBytes());
            Log.d(TAG,"Writing done ");
        }else {
            Toast.makeText(getApplicationContext(), "Failed to send data", Toast.LENGTH_SHORT).show();
            Log.d(TAG, "connected Thread" + connectedThread);
        }

    }
    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
       if( requestCode==REQUEST_BT_CONNECTION)
       {
                if (resultCode == Activity.RESULT_OK) {
                    // Bluetooth is now enabled
                    Log.d(TAG, "BT already enabled");
                   // makeDeviceDiscoverable();
                   // Log.d(TAG, "make discovarable");
                    connectDevice();
                } else {

                    Log.d(TAG, "BT not enabled");
                    turnonBT();//enable BT
                    // Toast.makeText(getApplicationContext(),"BT is not enabled",Toast.LENGTH_SHORT).show();
                    //finish();
                }


        }else{
           Toast.makeText(getApplicationContext(),"BT is not enabled",Toast.LENGTH_SHORT).show();
       }

    }
    private void connectDevice() {
        // Get the device MAC address
                String MAC ="20:16:08:10:48:56";
                Log.v(TAG, "Connect device MAC address" );
                BluetoothDevice bluetoothDevice = BA.getRemoteDevice(MAC);
                Log.v(TAG, "bluetoothDevice" + bluetoothDevice);
                // Initiate a connection request in a separate thread
               ConnectThread t = new ConnectThread(bluetoothDevice);
               t.start();
            }

    public class ConnectThread extends Thread {

        //private BluetoothAdapter btAdapter;
        private  final String TAG1 = "ConnectThread";
        private  final UUID MY_UUID = UUID.fromString("00001101-0000-1000-0000-00005F9834F8");
        //private final UUID MY_UUID = UUID.fromString("47731290-4b6e-454e-8c06-3f04dcb18eac");
        // public static final UUID MY_UUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
        private final BluetoothSocket mmSocket;
        private final BluetoothDevice mmDevice;

        public ConnectThread(BluetoothDevice device) {
            // Use a temporary object that is later assigned to mmSocket,
            // because mmSocket is final
            Log.v(TAG1,"Before creating Bluetooth Socket");
            BluetoothSocket tmp = null;
            mmDevice = device;

            // Get a BluetoothSocket to connect with the given BluetoothDevice
            Method m;
            try {
                BA.cancelDiscovery();
                tmp = device.createInsecureRfcommSocketToServiceRecord(MY_UUID);
                m = device.getClass().getMethod("createInsecureRfcommSocket", new Class[] {int.class});
                tmp = (BluetoothSocket) m.invoke(device, 1);
            } catch (IOException |  IllegalArgumentException | IllegalAccessException | InvocationTargetException | NoSuchMethodException  e) {
                // TODO Auto-generated catch block
                Log.i("BluetoothConnect", e.toString());
                e.printStackTrace();
            }

            mmSocket = tmp;
            // mHandler.obtainMessage(SUCCESS_CONNECT,mmSocket).sendToTarget();
        }

        public void run() {
            // Cancel discovery because it will slow down the connection
            BA.cancelDiscovery();

            try {
                // Connect the device through the socket. This will block
                // until it succeeds or throws an exception
                mmSocket.connect();
                connectsuccess=1;

            } catch (IOException connectException) {
                // Unable to connect; close the socket and get out
                try {
                    mmSocket.close();
                } catch (IOException closeException) {
                }


            }
            if(connectsuccess==1) {
                Log.d(TAG1,"Call connectedThread "+connectsuccess);
               callConnectedThread(mmSocket);
            }
        }
        public void cancel() {
            try {
                mmSocket.close();
            } catch (IOException e) {
                Log.e(TAG1, "close() of connect " + " socket failed", e);
            }
        }


    }

    public class ConnectedThread extends Thread {
        private final BluetoothSocket mmSocket;
        private final InputStream mmInStream;
        private final OutputStream mmOutStream;
        private  final String TAG2 = "ConnectedThread";


        public ConnectedThread(BluetoothSocket socket) {
            mmSocket = socket;
            InputStream tmpIn = null;
            OutputStream tmpOut = null;
            Log.d(TAG2,"In connected Thread");
            // Get the input and output streams, using temp objects because
            // member streams are final
            try {
                tmpIn = socket.getInputStream();
                tmpOut = socket.getOutputStream();
            } catch (IOException e) { }

            mmInStream = tmpIn;
            mmOutStream = tmpOut;
        }

        public void run() {
            byte[] buffer ;//=new byte[1024];  // buffer store for the stream
            int bytes; // bytes returned from read()

            // Keep listening to the InputStream until an exception occurs
            while (true) {
                try {
                    // Read from the InputStream
                    buffer=new byte[1024];
                    bytes = mmInStream.read(buffer);
                    // Send the obtained bytes to the UI activity
                   // mHandler.obtainMessage(MESSAGE_READ, bytes, -1, buffer)
                     //       .sendToTarget();
                } catch (IOException e) {
                    break;
                }
            }
        }

        /* Call this from the main activity to send data to the remote device */
        public void write(byte[] bytes) {
            try {
                Log.d(TAG2,"In write block");
                mmOutStream.write(bytes);
            } catch (IOException e) { }
        }

        /* Call this from the main activity to shutdown the connection */
        public void cancel() {
            try {
                mmSocket.close();
            } catch (IOException e) { }
        }
    }

}