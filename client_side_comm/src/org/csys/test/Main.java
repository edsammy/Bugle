package org.csys.test;

import gnu.io.PortInUseException;
import gnu.io.UnsupportedCommOperationException;

import java.io.IOException;
import java.util.TooManyListenersException;

public class Main
{
		
	public static void main(String[] args)
	{	
		
		TestProject proj = null;
		try{
			proj = new TestProject();
		}catch(UnsupportedCommOperationException e){
			e.printStackTrace();
		}catch(PortInUseException e){
			e.printStackTrace();
		}catch(IOException e){
			e.printStackTrace();
		}catch(TooManyListenersException e){
			e.printStackTrace();
		}finally{
			if(proj == null){
				System.err.println("Could not open COM port");
				System.exit(1);
			}
		}
		
		long l1 = System.currentTimeMillis();
		while(System.currentTimeMillis() - l1 < 20000);
		
		proj.close();
		
	}
	
}