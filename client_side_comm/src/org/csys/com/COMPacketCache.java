package org.csys.com;

public class COMPacketCache
{

	private static final int DEFAULT_SIZE = 32;
	
	private int index;
	private COMPacket[] cache;
	
	public COMPacketCache(COMPacket p)
	{
		this(p, DEFAULT_SIZE);
	}
	public COMPacketCache(COMPacket p, int n)
	{
		index = -1;
		cache = new COMPacket[n];
		try{
			for(int i = 0; i < cache.length; i++)
			{
				cache[i] = p.getClass().newInstance();
			}
		}catch(InstantiationException e){
			e.printStackTrace();
		}catch(IllegalAccessException e){
			e.printStackTrace();
		}
	}
	
	public COMPacket getFreePacket()
	{
		if(++index == cache.length)
			index = 0;
		return cache[index];
	}
	
}
