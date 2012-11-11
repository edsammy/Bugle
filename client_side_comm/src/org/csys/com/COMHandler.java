package org.csys.com;

import gnu.io.PortInUseException;
import gnu.io.UnsupportedCommOperationException;

import java.io.IOException;
import java.util.TooManyListenersException;

public class COMHandler
{
	
	private static final int PACKET_CACHE_SIZE = 64;

	private COMDataStream comStream;
	private COMPacketCache[] packetCache;
	private COMReadHandler readHandler;
	private COMWriteHandler writeHandler;
	
	public COMHandler(int bps) throws UnsupportedCommOperationException, PortInUseException, IOException, TooManyListenersException
	{
		comStream = new COMDataStream(bps);
		readHandler = new COMReadHandler(comStream);
		writeHandler = new COMWriteHandler(comStream);
		packetCache = new COMPacketCache[16];
	}
	
	public COMPacket getFreePacket(int i)
	{
		return packetCache[i].getFreePacket();
	}
	
	public void writePacket(COMPacket p)
	{
		writeHandler.writePacket(p);
	}
	
	public void setPacketListener(COMPacketListener l)
	{
		readHandler.setPacketListener(l);
	}

	public void registerPacket(COMPacket p)
	{
		packetCache[p.getID()] = new COMPacketCache(p, PACKET_CACHE_SIZE);
		readHandler.registerPacket(p);
	}
	
	public void close()
	{
		writeHandler.close();
		comStream.close();
	}
	
}
