<%@ page language="java" contentType="text/html; charset=ISO-8859-1"
    pageEncoding="ISO-8859-1"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<title>title</title>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
<!-- <link rel="stylesheet" href="http://cdn.bootcss.com/twitter-bootstrap/3.0.3/css/bootstrap.min.css"> -->
<!-- <script src="http://cdn.bootcss.com/jquery/1.10.2/jquery.min.js"></script> -->
<!-- <script src="http://cdn.bootcss.com/twitter-bootstrap/3.0.3/js/bootstrap.min.js"></script> -->
</head>
<body>
<%
String path = request.getScheme()+"://"+request.getHeader("host")+request.getContextPath();
String[] fileName = (String[])request.getAttribute("files");
String pathName = (String) request.getAttribute("path");
%>
<%-- <button type="button" class="btn btn-success" onclick="window.location.href='<%=path%>folderList.do?path=<%=pathName%>'">RETURN</button><br/> --%>
<table><tr><td><div>
<%=pathName
//out.println("<a href='"+path+"/taskList.do'><font color='red'>RETURN</font></a><br/>");
%>
</div></td><tr></tr><td><div>
<%
if(fileName != null){
for(int i=0;i<fileName.length;i++){
	if(fileName[i].endsWith("...")){
		out.println(""+fileName[i]+"<br/>");
	}
	else{
	    out.println("<a href='"+path+"/result/"+request.getParameter("path")+"/"+fileName[i]+"' target='blank_'>"+fileName[i]+"</a><br/>");
	}
}
}
%>
</div></td></tr>
</table>
</body>
</html>