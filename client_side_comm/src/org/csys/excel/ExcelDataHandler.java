package org.csys.excel;

import org.apache.poi.hssf.usermodel.HSSFCell;
import org.apache.poi.hssf.usermodel.HSSFRow;
import org.apache.poi.hssf.usermodel.HSSFSheet;
import org.apache.poi.hssf.usermodel.HSSFWorkbook;

public class ExcelDataHandler
{
	
	private HSSFWorkbook workbook;
    private HSSFSheet sheet;
    private HSSFRow row;
    private HSSFCell cell;
    
    private int index = 0;
    
    public ExcelDataHandler(String s, String s0, String s1, HSSFWorkbook wb)
    {
    	this(s, null, s0, s1, wb);
    }
    public ExcelDataHandler(String s, String s0, String s1, String s2, HSSFWorkbook wb)
    {
    	
    	workbook = wb;
    	sheet = workbook.createSheet(s);	
    	row = sheet.createRow(index++);
    	
    	int cellIndex = 0;
    	
    	if(s0 != null)
    	{
    		cell = row.createCell(cellIndex++);
        	cell.setCellType(HSSFCell.CELL_TYPE_STRING);
        	cell.setCellValue(s0);    		
    	}

    	cell = row.createCell(cellIndex++);
    	cell.setCellType(HSSFCell.CELL_TYPE_STRING);
    	cell.setCellValue(s1);
    	
    	cell = row.createCell(cellIndex++);
    	cell.setCellType(HSSFCell.CELL_TYPE_STRING);
    	cell.setCellValue(s2);
    	
    }
    
    public void sort()
    {
    	
    }

    public void addData(long l, double d)
    {
    	
    	row = sheet.createRow(index++);
    	
    	cell = row.createCell(HSSFCell.CELL_TYPE_NUMERIC);
    	cell.setCellValue(l);

    	cell = row.createCell(1);
    	cell.setCellType(HSSFCell.CELL_TYPE_NUMERIC);
    	cell.setCellValue(d);
    	
    }
    
    
    public void addData(long l, double d, double d1)
    {
    	
    	row = sheet.getRow(index++);
    	
    	cell = row.getCell(0);
    	cell.setCellType(HSSFCell.CELL_TYPE_NUMERIC);
    	cell.setCellValue(l);
    	
    	cell = row.getCell(1);
    	cell.setCellType(HSSFCell.CELL_TYPE_NUMERIC);
    	cell.setCellValue(d);

    	cell = row.getCell(2);
    	cell.setCellType(HSSFCell.CELL_TYPE_NUMERIC);
    	cell.setCellValue(d1);
    }
    
}
