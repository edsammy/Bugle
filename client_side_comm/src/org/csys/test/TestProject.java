package org.csys.test;

import gnu.io.PortInUseException;
import gnu.io.UnsupportedCommOperationException;

import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.TooManyListenersException;

import org.apache.poi.hssf.usermodel.HSSFWorkbook;
import org.csys.com.*;
import org.csys.excel.ExcelDataHandler;
import org.csys.util.TimeStamp;

public class TestProject implements COMPacketListener
{
	
	private COMHandler handler;
	
	private HSSFWorkbook workbook;
	private ExcelDataHandler wheelVsEngineSpeed;
	private ExcelDataHandler tempVsTime;
	
	public TestProject() throws UnsupportedCommOperationException, PortInUseException, IOException, TooManyListenersException
	{
		
		workbook = new HSSFWorkbook();
		wheelVsEngineSpeed = new ExcelDataHandler("Wheel Vs. Engine Speed", 
				"Time", 
				"Wheel Speed", 
				"Engine Speed", 
				workbook);
		tempVsTime = new ExcelDataHandler("Temperature vs. Time", 
				"Time", 
				"Temperature", 
				workbook);
		
		handler = new COMHandler(57600);
		
		handler.registerPacket(new COM0Packet());
		handler.registerPacket(new COM1Packet());
		handler.registerPacket(new COM2Packet());
		handler.registerPacket(new COM15Packet());
		
		handler.setPacketListener(this);
			
	}
	
	public void close()
	{
		
		handler.close();
		
		String date = new SimpleDateFormat("MM-dd").format(new Date(System.currentTimeMillis()));
		
		FileOutputStream fos = null;
		try{
			fos = new FileOutputStream("BajaData_"+ date + ".xls");
			workbook.write(fos);
			fos.close();
		}catch(FileNotFoundException e){
			e.printStackTrace();
		}catch(IOException e){
			e.printStackTrace();
		}
		
	}
	
	@Override
	public void processPacket(COMPacket packet)
	{
		System.out.println("\n" + packet.getName() + " Received");
		switch(packet.getID())
		{
		case 0:
			processWheelSpeedPacket((COM0Packet)packet);
			break;
		case 1:
			processTemperaturePacket((COM1Packet)packet);
			break;
		case 2:
			processTestSendPacket((COM2Packet)packet);
			break;
		case 15:
			processDebugPacket((COM15Packet)packet);
			break;	
		}	
	}
	
	public void processDebugPacket(COM15Packet p)
	{
		System.err.println("\tMessage: " + p.getText());
	}
	
	public void processWheelSpeedPacket(COM0Packet packet)
	{
		
		System.out.println("\tWheel Speed: " + packet.getWheelSpeed());
		System.out.println("\tEngine Speed: " + packet.getEngineSpeed());
		System.out.println("\tTime Stamp: " + packet.getTimeStamp());
	
		wheelVsEngineSpeed.addData(packet.getTimeStamp().toMillis(), 
				packet.getWheelSpeed(), 
				packet.getEngineSpeed());
		
	}

	public void processTemperaturePacket(COM1Packet packet)
	{
		
		System.out.println("\tTemperature: " + packet.getTemperature());
		System.out.println("\tTime Stamp: " + packet.getTimeStamp());	
	
		tempVsTime.addData(packet.getTimeStamp().toMillis(), 
				packet.getTemperature());
		
	}

	public void processTestSendPacket(COM2Packet packet)
	{
		
		System.out.println("\tVariable: " + packet.getVar());
		System.out.println("\tTime Stamp: " + packet.getTimeStamp());
		
		packet.setVar(24);
		TimeStamp.update(packet.getTimeStamp());
		handler.writePacket(packet);
		
	}
	
}
