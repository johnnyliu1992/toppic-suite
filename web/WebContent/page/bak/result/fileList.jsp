<%@ page language="java" contentType="text/html; charset=ISO-8859-1"
    pageEncoding="ISO-8859-1"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
<title>Insert title here</title>
</head>
<body>
<%
String path = request.getScheme()+"://"+request.getHeader("host")+request.getContextPath()+"/result/"+request.getParameter("path")+"/";
String[] fileName = (String[])request.getAttribute("files");
if(fileName != null){
for(int i=0;i<fileName.length;i++){
	out.println("<a href='"+path+fileName[i]+"'>"+fileName[i]+"</a><br/>");
}
}
%>
</body>
</html>