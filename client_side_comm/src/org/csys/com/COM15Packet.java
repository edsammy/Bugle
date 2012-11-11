package org.csys.com;

import java.io.IOException;
import java.io.OutputStream;

import org.csys.util.DynByteBuffer;

public class COM15Packet extends COMPacket
{

	private String text;
		
	public String getText()
	{
		return text;
	}
	
	@Override
	public int getID()
	{
		return 15;
	}

	@Override
	public int getLength()
	{
		return 4;
	}

	@Override
	public boolean isVariableLength()
	{
		return true;
	}
	
	@Override
	public String getName()
	{
		return "Debug Packet";
	}

	@Override
	public void write(OutputStream os) throws IOException
	{
		
	}

	@Override
	public boolean decode(DynByteBuffer buffer)
	{
		try{
			int n = decodeInt16(buffer);
			text = "";
			for(int i = 0; i < n; i++)
			{
				text += (char)decodeByte(buffer);
			}	
		}catch(NumberFormatException e){
			return false;
		}
		return true;
	}

}
