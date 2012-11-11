package org.csys.com;

import java.io.IOException;
import java.io.OutputStream;

import org.csys.util.DynByteBuffer;
import org.csys.util.TimeStamp;

public class COM0Packet extends COMPacket
{
	
	private float wheelSpeed, engineSpeed;
	private TimeStamp timeStamp = new TimeStamp();
	
	public float getWheelSpeed()
	{
		return wheelSpeed;
	}
	
	public float getEngineSpeed()
	{
		return engineSpeed;
	}
	
	public TimeStamp getTimeStamp()
	{
		return timeStamp;
	}
	
	@Override
	public int getID() 
	{
		return 0;
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
			wheelSpeed = decodeFloat32(buffer);	
			engineSpeed = decodeFloat32(buffer);	
			decodeTimeStamp(buffer, timeStamp);
		}catch(NumberFormatException e){
			return false;
		}
		return true;
	}

	@Override
	public String getName()
	{
		return "Wheel Speed Packet";
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
