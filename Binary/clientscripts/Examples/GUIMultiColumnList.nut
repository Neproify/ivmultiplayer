local listColumn;
local sizeScreen = guiGetScreenSize();

listColumn = GUIMultiColumnList();
listColumn.setSize(900.0,500.0,false);
listColumn.setPosition(sizeScreen[0]/2-450.0,sizeScreen[1]/2-250.0,false);

// Add columns to the list

listColumn.setProperty("ColumnHeader", "text:Column1 width:{0.5,0} id:0");
listColumn.setProperty("ColumnHeader", "text:Column2 width:{0.2,0} id:1");
listColumn.setProperty("ColumnHeader", "text:Column3 width:{0.2,0} id:2");

// Add rows

	/* Row 1 */
local row1 = listColumn.addRow(); // addRow() returns the ID of the row created
listColumn.setItem("Column 1 Value!", 0, row1);
listColumn.setItem("Column 2 Value!", 1, row1);
listColumn.setItem("Column 3 Value!", 2, row1);
	/* Row 2 */
local row2 = listColumn.addRow(); // addRow() returns the ID of the row created
listColumn.setItem("Here's another row !", 0, row2);
listColumn.setItem("Here's another row !", 1, row2);
listColumn.setItem("Here's another row !", 2, row2);

/* The list is now done. Now we create the event rowClick to check if a user click a row */

addEvent("rowClick",
	function(tableName, rowID)
	{
		if(tableName == listColumn.getName()) // Do the clicked row belongs to our table ?
		{
			if(rowID == row1)
				addChatMessage("You clicked the first row!");
			if(rowID == row2)
				addChatMessage("You clicked the secondrow!");
		}
		return 1;
	}
);