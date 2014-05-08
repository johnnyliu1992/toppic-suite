<%@ page language="java" contentType="text/html; charset=GB18030"
    pageEncoding="UTF-8"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=GB18030">
<title>Database Upload</title>
</head>
<body>
<h2>Database Upload</h2>

<form action="DatabaseFile.do" method="post" enctype="multipart/form-data">
	database file:<input type="file" name="file">
	<input type="submit" value="submit"> 
</form>

</body>
</html>