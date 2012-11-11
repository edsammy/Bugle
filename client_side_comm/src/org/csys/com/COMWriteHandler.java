package org.csys.com;

import java.io.IOException;
import java.io.OutputStream;
import java.util.LinkedList;
import java.util.Queue;

public class COMWriteHandler
{

	private WriteThread writeThread;
	private OutputStream outputStream;
	private volatile Queue<COMPacket> packetQueue;
	
	public COMWriteHandler(COMDataStream s)
	{
		writeThread = new WriteThread();
		packetQueue = new LinkedList<>();
		outputStream = s.getOutputStream();
	}
	
	public void writePacket(COMPacket p)
	{
		synchronized(packetQueue)
		{
			packetQueue.add(p);
			synchronized(writeThread)
			{
				writeThread.notifyAll();		
			}
		}
	}
	
	public void close()
	{
		writeThread.isClosing = true;
		synchronized(writeThread)
		{
			writeThread.notifyAll();
		}
		while(writeThread.isRunning);
	}
	
	class WriteThread extends Thread
	{
		
		public volatile boolean isRunning;
		public volatile boolean isClosing;
		
		public void run()
		{
			isRunning = true;
			while(!isClosing)
			{
				synchronized(packetQueue)
				{
					if(!packetQueue.isEmpty())
					{
						COMPacket packet = packetQueue.poll();
						try{
							packet.write(outputStream);
						}catch(IOException e){
							e.printStackTrace();
						}
					}
				}
				try{
					wait();
				}catch(InterruptedException e){
					e.printStackTrace();
				}
			}
			isRunning = false;
		}
		
	}
	
}
