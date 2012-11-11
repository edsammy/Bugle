package org.csys.com;

import java.io.IOException;
import java.io.OutputStream;

import org.csys.util.DynByteBuffer;
import org.csys.util.TimeStamp;

public class COM1Packet extends COMPacket
{
	
	private int temperature;
	private TimeStamp timeStamp = new TimeStamp();
	
	public int getTemperature()
	{
		return temperature;
	}
	
	public TimeStamp getTimeStamp()
	{
		return timeStamp;
	}
	
	@Override
	public int getID() 
	{
		return 1;
	}

	@Override
	public int getLength()
	{
		return 8;
	}

	@Override
	public boolean decode(DynByteBuffer buffer)
	{
		try{
			temperature = decodeInt16(buffer);
			decodeTimeStamp(buffer, timeStamp);
		}catch(NumberFormatException e){
			return false;
		}
		return true;
	}

	@Override
	public String getName()
	{
		return "Temperature Packet";
	}

	@Override
	public void write(OutputStream os) throws IOException
	{
		
	}
	
	@Override
	public boolean isVariableLength()
	{
		return false;
	}
	
}
