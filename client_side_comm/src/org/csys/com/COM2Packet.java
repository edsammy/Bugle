package org.csys.com;

import java.io.IOException;
import java.io.OutputStream;

import org.csys.util.DynByteBuffer;
import org.csys.util.TimeStamp;

public class COM2Packet extends COMPacket
{

	private byte var;
	private TimeStamp timeStamp = new TimeStamp();

	public TimeStamp getTimeStamp()
	{
		return timeStamp;
	}
	
	public void setVar(int i)
	{
		var = (byte)i;
	}
	
	public int getVar()
	{
		return var;
	}
	
	@Override
	public int getID()
	{
		return 2;
	}

	@Override
	public int getLength()
	{
		return 12;
	}

	@Override
	public String getName()
	{
		return "Send Test Packet";
	}

	@Override
	public void write(OutputStream os) throws IOException
	{
		TimeStamp.update(timeStamp);
		os.write(var);
		writeTimeStamp(os, timeStamp);
	}

	@Override
	public boolean decode(DynByteBuffer buffer)
	{
		try{
			var = (byte)decodeByte(buffer);
			decodeTimeStamp(buffer, timeStamp);
		}catch(NumberFormatException e){
			return false;
		}
		return true;
	}

	@Override
	public boolean isVariableLength()
	{
		return false;
	}
	
}
