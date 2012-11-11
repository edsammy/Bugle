package org.csys.util;

import java.nio.ByteOrder;
import java.util.Arrays;

public class DynByteBuffer
{

	private ByteOrder byteOrder;
	private int mark;
	private byte[] buffer;
	
	public DynByteBuffer(int i, ByteOrder b)
	{
		mark = 0;
		byteOrder = b;
		buffer = new byte[i];
		Arrays.fill(buffer, (byte)-1);
	}
	
	public byte[] getBuffer()
	{
		return buffer;
	}
	
	public float getFloat()
	{
		int i = getInt();
		return Float.intBitsToFloat(i);
	}
	
	public int getByte()
	{
		return buffer[mark++];
	}
	
	public void incMark(int i)
	{
		mark += i;
	}
	
	public int getInt()
	{
		int data = 0;
		if(byteOrder == ByteOrder.LITTLE_ENDIAN)
		{
			data |= buffer[mark++] & 0xff;
			data |= (buffer[mark++] & 0xff) << 8;
			data |= (buffer[mark++] & 0xff) << 16;
			data |= (buffer[mark++] & 0xff) << 24;
		}
		else
		{
			data |= (buffer[mark++] & 0xff) << 24;			
			data |= (buffer[mark++] & 0xff) << 16;
			data |= (buffer[mark++] & 0xff) << 8;
			data |= buffer[mark++] & 0xff;
		}
		return data;
	}
	
	public int getMark()
	{
		return mark;
	}
	
	public void setMark(int i)
	{
		mark = i;
	}
	
	public void rewind()
	{
		mark = 0;
	}
	
	public void decMark(int di)
	{
		mark -= di;
	}
	
	private void incBuffer()
	{
		byte[] temp = new byte[buffer.length * 2];
		System.arraycopy(buffer, 0, temp, 0, buffer.length);
		Arrays.fill(buffer, buffer.length, temp.length, (byte)-1);
		buffer = temp;
	}
	
	public void putFloat(float f)
	{
		int data = Float.floatToIntBits(f);
		putInt(data);
	}
	
	public void putBytes(byte[] b, int n)
	{
		if(mark + n >= buffer.length)
			incBuffer();
		System.arraycopy(b, 0, buffer, mark, n);
	}
	
	public void putBytes(byte[] b, int s, int n)
	{
		if(mark + n >= buffer.length)
			incBuffer();
		System.arraycopy(b, s, buffer, mark, n);
	}
	
	public boolean isEmpty()
	{
		return mark == 0;
	}
	
	public void putInt(int i)
	{
		if(mark + 4 >= buffer.length)
		{
			incBuffer();
		}
		if(byteOrder == ByteOrder.LITTLE_ENDIAN)
		{
			buffer[mark++] = (byte)(i & 0xff);
		    buffer[mark++] = (byte)((i >> 8) & 0xff);
		    buffer[mark++] = (byte)((i >> 16) & 0xff);
		    buffer[mark++] = (byte)((i >> 24) & 0xff);
		}
		else
		{
		    buffer[mark++] = (byte)((i >> 24) & 0xff);
		    buffer[mark++] = (byte)((i >> 16) & 0xff);
			buffer[mark++] = (byte)((i >> 8) & 0xff);
		    buffer[mark++] = (byte)(i & 0xff);
		}
	}
	
	public void putByte(int i)
	{
		if(mark + 1 == buffer.length)
		{
			incBuffer();
		}
		buffer[mark++] = (byte)i;
	}

}
