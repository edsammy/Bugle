package org.csys.com;

import java.io.IOException;
import java.io.OutputStream;
import java.nio.ByteOrder;

import org.csys.util.DynByteBuffer;
import org.csys.util.TimeStamp;

public abstract class COMPacket
{

	private static ByteOrder BYTE_ORDER = ByteOrder.nativeOrder();
	
	public static final int START = 10;
	public static final int END = 13;
	
	protected final void writeFloat32(OutputStream os, float f) throws IOException
	{
		byte b;
		int bits = Float.floatToIntBits(f);
		if(BYTE_ORDER == ByteOrder.LITTLE_ENDIAN)
		{
			for(int i = 0; i < 8; i++)
			{
				b = (byte)((bits >> (4 * i)) & 0xf);	
				if(b < 10) os.write(48 + b);
				else os.write(65 + b);
			}	
		}
		else
		{
			for(int i = 8; i > 0; i++)
			{
				b = (byte)((bits >> (4 * i)) & 0xf);	
				if(b < 10) os.write(48 + b);
				else os.write(65 + b);
			}
		}
	}
	
	protected final void writeByte(OutputStream os, int b) throws IOException
	{
		b = (byte)(b & 0xf);	
		if(b < 10) os.write(48 + b);
		else os.write(65 + b);	
		b = (byte)((b >> 4) & 0xf);	
		if(b < 10) os.write(48 + b);
		else os.write(65 + b);	
	}
	
	protected final void writeTimeStamp(OutputStream os, TimeStamp t) throws IOException
	{
		writeByte(os, t.hours());
		writeByte(os, t.minutes());
		writeByte(os, t.seconds());
		writeInt16(os, t.milleseconds());
	}
	
	protected final void writeInt16(OutputStream os, int i) throws IOException
	{
		if(BYTE_ORDER == ByteOrder.LITTLE_ENDIAN)
		{
			os.write(i & 0xff);
			os.write((i & 0xff) << 8);
		}
		else
		{
			os.write((i & 0xff) << 8);
			os.write(i & 0xff);
		}
	}
	
	protected final int decodeByte(DynByteBuffer b) throws NumberFormatException
	{
		int b0, b1;
		int bits = 0;
		b1 = b.getByte();
		b0 = b.getByte();
		if(b1 >= 48 && b1 <= 57)
			bits |= ((b1 - 48) & 0x0f) << 4;		
		else if(b1 >= 65 && b1 <= 70)
			bits |= ((b1 - 55) & 0x0f) << 4;
		else
			throw new NumberFormatException();
		if(b0 >= 48 && b0 <= 57)
			bits |= (b0 - 48) & 0x0f;
		else if(b0 >= 65 && b0 <= 70)
			bits |= (b0 - 55) & 0x0f;
		else
			throw new NumberFormatException();
		return bits;
	}
	
	protected final int decodeInt16(DynByteBuffer b) throws NumberFormatException
	{
		int byte0 = decodeByte(b);
		int byte1 = decodeByte(b);
		int bits = 0;
		if(BYTE_ORDER == ByteOrder.LITTLE_ENDIAN)
		{
			bits |= byte0 & 0xff;
			bits |= (byte1 & 0xff) << 8;
		}
		else
		{
			bits |= (byte1 & 0xff) << 8;
			bits |= byte0 & 0xff;
		}
		return bits;
	}
	
	private int hrs, mins, secs, ms;
	protected final void decodeTimeStamp(DynByteBuffer b, TimeStamp t) throws NumberFormatException
	{
		hrs = decodeByte(b);
		mins = decodeByte(b);
		secs = decodeByte(b);
		ms = decodeInt16(b);
		t.setData(hrs, mins, secs, ms);
	}
	
	protected final float decodeFloat32(DynByteBuffer b) throws NumberFormatException
	{
		int byte0 = decodeByte(b);
		int byte1 = decodeByte(b);
		int byte2 = decodeByte(b);
		int byte3 = decodeByte(b);
		int bits = 0;
		if(BYTE_ORDER == ByteOrder.LITTLE_ENDIAN)
		{
			bits |= byte0 & 0xff;
			bits |= (byte1 & 0xff) << 8;
			bits |= (byte2 & 0xff) << 16;
			bits |= (byte3 & 0xff) << 24;
		}
		else
		{
			bits |= (byte3 & 0xff) << 24;
			bits |= (byte2 & 0xff) << 16;
			bits |= (byte1 & 0xff) << 8;
			bits |= byte0 & 0xff;
		}
		return Float.intBitsToFloat(bits);
	}
	
	public abstract boolean isVariableLength();
	public abstract int getID();
	public abstract int getLength();
	public abstract String getName();
	public abstract void write(OutputStream os) throws IOException;
	public abstract boolean decode(DynByteBuffer b);
	
}
