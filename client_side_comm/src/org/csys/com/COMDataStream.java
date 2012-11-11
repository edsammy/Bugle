package org.csys.com;

import gnu.io.CommPortIdentifier;
import gnu.io.PortInUseException;
import gnu.io.SerialPort;
import gnu.io.SerialPortEventListener;
import gnu.io.UnsupportedCommOperationException;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Enumeration;
import java.util.TooManyListenersException;

public class COMDataStream
{
	
	private static final String[] PORT_NAMES = { 
		"COM3", 
		"/dev/ttyUSB0", 
		"/dev/tty.usbserial-A9007UX1"
	};
	
	private static final int TIME_OUT = 2000;

	private SerialPort serialPort;
	
	@SuppressWarnings("rawtypes")
	public COMDataStream(int bps) throws UnsupportedCommOperationException, PortInUseException, IOException
	{
		
		CommPortIdentifier portId = null;
		Enumeration portEnum = CommPortIdentifier.getPortIdentifiers();
		a: while(portEnum.hasMoreElements())
		{
			CommPortIdentifier currPortId = (CommPortIdentifier)portEnum.nextElement();
			for(String s : PORT_NAMES)
			{
				if(currPortId.getName().equals(s))
				{
					portId = currPortId;
					break a;
				}	
			}
		}
		
		serialPort = (SerialPort)portId.open(this.getClass().getName(), TIME_OUT);
		serialPort.setSerialPortParams(bps, SerialPort.DATABITS_8, SerialPort.STOPBITS_1, SerialPort.PARITY_NONE);
		serialPort.notifyOnDataAvailable(true);

	}
	
	public OutputStream getOutputStream()
	{
		try{
			return serialPort.getOutputStream();
		}catch(IOException e){
			e.printStackTrace();
		}
		return null;
	}
	
	public InputStream getInputStream()
	{
		try{
			return serialPort.getInputStream();
		}catch(IOException e){
			e.printStackTrace();
		}
		return null;
	}
	
	public synchronized void addEventListener(SerialPortEventListener l) throws TooManyListenersException
	{
		serialPort.addEventListener(l);
	}
	
	public synchronized void close()
	{
		serialPort.removeEventListener();
		serialPort.close();
	}
		
}
