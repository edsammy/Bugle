package org.csys.util;

public class TimeStamp
{

	private static long BASE_TIME;
	private static long interval;
	private static long currTime;
	
	static
	{
		BASE_TIME = System.currentTimeMillis();
	}
	
	private byte hrs;
	private byte mins;
	private byte secs;
	private short ms;
	
	public void setData(int hrs, int mins, int secs, int ms)
	{
		this.hrs = (byte)hrs;
		this.mins = (byte)mins;
		this.secs = (byte)secs;
		this.ms = (short)ms;
	}
	
	public void setData(long l)
	{
		this.hrs = (byte)(l / 3600000L);
		l -= 3600000L * hrs;
		this.mins = (byte)(l / 60000L);
		l -= mins * 60000L;
		this.secs = (byte)(l / 1000L);
		l -= secs * 1000L;
		this.ms = (short)l;
	}
	
	public String toString()
	{
		return hrs + ":" + mins + ":" + secs + "." + ms;
	}
	
	public int hours()
	{
		return hrs;
	}
	
	public int minutes()
	{
		return mins;
	}
	
	public int seconds()
	{
		return secs;
	}
	
	public int milleseconds()
	{
		return ms;
	}
	
	public static void update(TimeStamp t)
	{
		currTime = System.currentTimeMillis();
		interval = currTime - BASE_TIME;
		t.setData(interval);
	}
	
	public long toMillis()
	{
		return (long)hrs * 3600000L + 
				(long)mins * 60000L + 
				(long)secs * 10000L + 
				(long)ms;
	}
	
}
