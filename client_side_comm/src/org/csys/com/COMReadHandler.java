package org.csys.com;

import gnu.io.SerialPortEvent;
import gnu.io.SerialPortEventListener;

import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteOrder;
import java.util.TooManyListenersException;
 
import org.csys.util.DynByteBuffer;

public class COMReadHandler implements SerialPortEventListener
{

	private COMDataStream stream;
	private InputStream inputStream;
	private COMPacket[] packetMap;
	private DynByteBuffer dataBuffer; 

	private int currPacketID;
	private boolean isNextBytePacketID;
	
	private COMPacketListener packetListener;

	public COMReadHandler(COMDataStream s) throws TooManyListenersException 
	{
		stream = s;
		packetMap = new COMPacket[16];
		stream.addEventListener(this);
		inputStream = stream.getInputStream();
		dataBuffer = new DynByteBuffer(64, ByteOrder.nativeOrder());
		isNextBytePacketID = false;
		currPacketID = -1;
	}
	
	public void setPacketListener(COMPacketListener l)
	{
		packetListener = l;
	}
	
	public boolean registerPacket(COMPacket p)
	{
		if(packetMap[p.getID()] != null)
		{
			System.err.println("Packet ID (" + p.getID() + ") already taken!");
			return false;
		}
		packetMap[p.getID()] = p;
		return true;
	}
	
	@Override
	public void serialEvent(SerialPortEvent e)
	{
		if(e.getEventType() == SerialPortEvent.DATA_AVAILABLE)
		{
			int i;
			try{
				while((i = inputStream.read()) != -1)
				{
					if(i == COMPacket.START)
					{
						if(currPacketID != -1)
						{
							System.err.println("Packet (" + currPacketID + ") Lost");
						}
						currPacketID = -1;
						isNextBytePacketID = true;
					}
					else if(i == COMPacket.END)
					{
						if(currPacketID == -1)
							continue;
						if(!packetMap[currPacketID].isVariableLength() && dataBuffer.getMark() != packetMap[currPacketID].getLength())
							System.err.println("Data Loss in Packet (" + currPacketID + ")");
						dataBuffer.rewind();
						if(packetMap[currPacketID].decode(dataBuffer) && 
								packetListener != null)
							packetListener.processPacket(packetMap[currPacketID]);
						else
							System.err.println("Corrupt Packet (" + currPacketID + ")");
						currPacketID = -1;
					}
					else if(isNextBytePacketID)
					{
						if(i < 0 || i >= packetMap.length || packetMap[i] == null)
						{
							System.err.println("Packet ID (" + i + ") Invalid");
							currPacketID = -1;
							isNextBytePacketID = false;
							continue;
						}
						dataBuffer.rewind();
						currPacketID = i;
						isNextBytePacketID = false;
					}
					else
					{
						if(currPacketID == -1)
						{
							System.err.println("Packetless Data Lost");
							continue;
						}
						dataBuffer.putByte(i);
					}
				}
			}catch(IOException e1){
				e1.printStackTrace();
			}
		}
	}
	
}
