macro notrynohigh_get_filename(pathName)
{
	nlength = strlen(pathName)
	i = nlength - 1
	name = ""
	while (1)
	{
		ch = pathName[i]
		if ("\\" == "@ch@")
			break
		i = i - 1
	}
	i = i + 1
	while (i < nlength)
	{
		name = cat(name, pathName[i])
	}
	return name	
}

macro notrynohigh_file_header()
{
	// get aurthor name
	szMyName = getenv(MYNAME)
	if (strlen(szMyName) <= 0)
	{
		szMyName = "notrynohigh"
	}

	// get company name
	szCompanyName = getenv(MYCOMPANY)
	if (strlen(szCompanyName) <= 0)
	{
		szCompanyName = szMyName
	}

	// get time
	szTime = GetSysTimme(True)
	Day = szTime.Day
	Month = szTime.Month
	Year - szTime.Year
	if (Day < 10)
	{
		szDay = "0@Day@"
	}
	else
	{
		szDay = Day
	}
	if (Month < 10)
	{
		szMonth = "0@Month@"
	}
	else
	{
		szMonth = Month
	}

	// get file name
	hbuf = GetCurrentBuf()
	szpathName = getBufName(hbuf)
	szfileName = notrynohigh_get_filename(szpathName)
	nlength = StrLen(szfileName)

	// assemble the string
	line_num = 0
	hbuf = GetBufName()
	InsBufLine(hbuf, line_num, "")
	line_num = line_num + 1
	InsBufLine(hbuf, line_num, "/*")
	line_num = line_num + 1	
	InsBufLine(hbuf, line_num, " *!")
	line_num = line_num + 1	
	InsBufLine(hbuf, line_num, " * \\file		:")
	line_num = line_num + 1	
	InsBufLine(hbuf, line_num, " * \\brief		:")
	line_num = line_num + 1	
	InsBufLine(hbuf, line_num, " *\\version		:")
	line_num = line_num + 1	
	InsBufLine(hbuf, line_num, " *\\data		: @Year@/@szMonth@/@szDay@")
	line_num = line_num + 1 
	InsBufLine(hbuf, line_num, " *\\author		: @szMyName@")
	line_num = line_num + 1
	InsBufLine(hbuf, line_num, " *Last modified by @szMyName@ @Year@/@szMonth@/@szDay@")
	line_num = line_num + 1	
	InsBufLine(hbuf, line_num, " *Copyright (c) @Year@ by NOTRYNOHIGH. ALL Rights Reserved.")
	line_num = line_num + 1
	InsBufLine(hbuf, line_num, " */")
	line_num = line_num + 1
	InsBufLine(hbuf, line_num, "   ")
	line_num = line_num + 1	
	InsBufLine(hbuf, line_num, "/*Includes ----------------------------------------------*/")
	line_num = line_num + 1
	InsBufLine(hbuf, line_num, "   ")
	line_num = line_num + 1	
	
	InsBufLine(hbuf, line_num, "/* \\defgroup XXXX_Private_TypesDefinitions")
	line_num = line_num + 1	
	InsBufLine(hbuf, line_num, " * \\{")
	line_num = line_num + 1		
	InsBufLine(hbuf, line_num, " */")
	line_num = line_num + 1	
	InsBufLine(hbuf, line_num, "   ")
	line_num = line_num + 1		
	InsBufLine(hbuf, line_num, "/**")
	line_num = line_num + 1		
	InsBufLine(hbuf, line_num, " * \\}")
	line_num = line_num + 1			
	InsBufLine(hbuf, line_num, " */")
	line_num = line_num + 1	
	InsBufLine(hbuf, line_num, "   ")
	line_num = line_num + 1		

	InsBufLine(hbuf, line_num, "/* \\defgroup XXXX_Private_Defines")
	line_num = line_num + 1	
	InsBufLine(hbuf, line_num, " * \\{")
	line_num = line_num + 1		
	InsBufLine(hbuf, line_num, " */")
	line_num = line_num + 1	
	InsBufLine(hbuf, line_num, "   ")
	line_num = line_num + 1		
	InsBufLine(hbuf, line_num, "/**")
	line_num = line_num + 1		
	InsBufLine(hbuf, line_num, " * \\}")
	line_num = line_num + 1			
	InsBufLine(hbuf, line_num, " */")
	line_num = line_num + 1	
	InsBufLine(hbuf, line_num, "   ")	
	line_num = line_num + 1		

	InsBufLine(hbuf, line_num, "/* \\defgroup XXXX_Private_Macros")
	line_num = line_num + 1	
	InsBufLine(hbuf, line_num, " * \\{")
	line_num = line_num + 1		
	InsBufLine(hbuf, line_num, " */")
	line_num = line_num + 1	
	InsBufLine(hbuf, line_num, "   ")
	line_num = line_num + 1		
	InsBufLine(hbuf, line_num, "/**")
	line_num = line_num + 1		
	InsBufLine(hbuf, line_num, " * \\}")
	line_num = line_num + 1			
	InsBufLine(hbuf, line_num, " */")
	line_num = line_num + 1	
	InsBufLine(hbuf, line_num, "   ")
	line_num = line_num + 1		

	InsBufLine(hbuf, line_num, "/* \\defgroup XXXX_Private_Variables")
	line_num = line_num + 1	
	InsBufLine(hbuf, line_num, " * \\{")
	line_num = line_num + 1		
	InsBufLine(hbuf, line_num, " */")
	line_num = line_num + 1	
	InsBufLine(hbuf, line_num, "   ")
	line_num = line_num + 1		
	InsBufLine(hbuf, line_num, "/**")
	line_num = line_num + 1		
	InsBufLine(hbuf, line_num, " * \\}")
	line_num = line_num + 1			
	InsBufLine(hbuf, line_num, " */")
	line_num = line_num + 1	
	InsBufLine(hbuf, line_num, "   ")
	line_num = line_num + 1		

	InsBufLine(hbuf, line_num, "/* \\defgroup XXXX_Private_FunctionPrototypes")
	line_num = line_num + 1	
	InsBufLine(hbuf, line_num, " * \\{")
	line_num = line_num + 1		
	InsBufLine(hbuf, line_num, " */")
	line_num = line_num + 1	
	InsBufLine(hbuf, line_num, "   ")
	line_num = line_num + 1		
	InsBufLine(hbuf, line_num, "/**")
	line_num = line_num + 1		
	InsBufLine(hbuf, line_num, " * \\}")
	line_num = line_num + 1			
	InsBufLine(hbuf, line_num, " */")
	line_num = line_num + 1	
	InsBufLine(hbuf, line_num, "   ")
	line_num = line_num + 1		

	InsBufLine(hbuf, line_num, "/* \\defgroup XXXX_Private_Functions")
	line_num = line_num + 1	
	InsBufLine(hbuf, line_num, " * \\{")
	line_num = line_num + 1		
	InsBufLine(hbuf, line_num, " */")
	line_num = line_num + 1	
	InsBufLine(hbuf, line_num, "   ")
	line_num = line_num + 1		
	InsBufLine(hbuf, line_num, "/**")
	line_num = line_num + 1		
	InsBufLine(hbuf, line_num, " * \\}")
	line_num = line_num + 1			
	InsBufLine(hbuf, line_num, " */")
	line_num = line_num + 1	
	InsBufLine(hbuf, line_num, "   ")
	line_num = line_num + 1		

	InsBufLine(hbuf, line_num, "/* \\defgroup XXXX_Exported_Functions")
	line_num = line_num + 1	
	InsBufLine(hbuf, line_num, " * \\{")
	line_num = line_num + 1		
	InsBufLine(hbuf, line_num, " */")
	line_num = line_num + 1	
	InsBufLine(hbuf, line_num, "   ")
	line_num = line_num + 1		
	InsBufLine(hbuf, line_num, "/**")
	line_num = line_num + 1		
	InsBufLine(hbuf, line_num, " * \\}")
	line_num = line_num + 1			
	InsBufLine(hbuf, line_num, " */")
	line_num = line_num + 1	
	InsBufLine(hbuf, line_num, "   ")	
	line_num = line_num + 1		
	InsBufLine(hbuf, line_num, "/************************ (C) COPYRIGHT JUZHANGO *****END OF FILE****/")	
	line_num = line_num + 1			
}


macro

