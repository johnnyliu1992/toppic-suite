<%@ page language="java" contentType="text/html; charset=ISO-8859-1"
    pageEncoding="ISO-8859-1"%>
<%String name = (String)request.getAttribute("path"); %>
<%
String servpath = request.getContextPath();
String basePath = request.getScheme()+"://"+request.getServerName()+":"+request.getServerPort()+servpath+"/";
%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
<title>Zipping Files</title>
<link rel="stylesheet" href="<%=basePath %>page/css/bootstrap.min.css">
<script src="<%=basePath %>page/js/jquery.min.js"></script>
<script src="<%=basePath %>page/js/bootstrap.min.js"></script>
<script type="text/javascript">
var msalign_check_id = window.setInterval(refreshProcessingBar, 3000);
function refreshProcessingBar(){
	htmlobj=$.ajax({url:"zipover.do?path=<%=name %>",async:false});
	if(htmlobj.responseText=="1"){
		document.getElementById("info").innerHTML=document.getElementById("info").innerHTML+"<br/>Zipping is completed!<br/>Please click the following link to download the files!";
		document.getElementById("links").style.display="";
		window.clearTimeout(msalign_check_id); 
	}
	/*
	if(htmlobj.responseText.indexOf("create folder xml/species")!=-1){
		document.getElementById("runningbar").style.width="100%";
		window.clearTimeout(msalign_check_id); 
		document.getElementById("resultFrame").src="./folderList.do";
		document.getElementById("resultTab").click();
	}
	cur_process+=5;
	if(cur_process>=105){
		window.clearTimeout(msalign_check_id); 
	}*/
}

</script>
</head>
<body onload="refreshProcessingBar()">
<div id="info">Please wait a moment.<br/>Zipping the files ...</div>
<div id="links" style="display:none"><a href="result/<%=name%>/<%=name%>.zip"><%=name %>.zip</a></div>
</body>
</html>