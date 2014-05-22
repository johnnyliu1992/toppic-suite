<%@ page language="java" contentType="text/html; charset=ISO-8859-1"
    import="java.io.*"
    import="java.util.*"
    import="edu.iupui.iac.msalignplus.dao.*"
    pageEncoding="ISO-8859-1"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<%
String servpath = request.getContextPath();
String basePath = request.getScheme()+"://"+request.getServerName()+":"+request.getServerPort()+servpath+"/";
%>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
<title>Insert title here</title>
<link rel="stylesheet" href="<%=basePath %>page/css/bootstrap.min.css">
<script src="<%=basePath %>page/js/jquery.min.js"></script>
<script src="<%=basePath %>page/js/bootstrap.min.js"></script>
<script type="text/javascript">
function toDeleteResult(name,obj){
	if(obj.innerHTML=="&nbsp;&nbsp;Stop&nbsp;&nbsp;"){
		if(confirm("Do you want to stop this task?"))
		{
			document.getElementById('runningStats').value="1";
			document.getElementById('delName_id').value=name;
			document.getElementById('deleteFrm').submit();
			document.getElementById("BtnR"+ids).style.display="none";
			document.getElementById("BtnG"+ids).style.display="";
		}
	}
	else if(obj.innerHTML=="Delete"){
		if(confirm("Do you want to delete this task?"))
		{
			document.getElementById('runningStats').value="0";
			document.getElementById('delName_id').value=name;
			document.getElementById('deleteFrm').submit();
			document.getElementById("BtnR"+ids).style.display="none";
		}
	}
}
function refreshRow(ids){
	document.getElementById("TD"+ids).innerHTML="-";
	htmlobj=$.ajax({url:"runningPercent.do?name="+ids,async:false});
	var arr = htmlobj.responseText.split(',');
	document.getElementById("TD"+ids).innerHTML=arr[0];
	document.getElementById("TDC"+ids).innerHTML=arr[3];
	document.getElementById("P"+ids).innerHTML="("+arr[1]+")";
	if((arr[1]=="finished"&&arr[2]=="1")||arr[1]=="waiting"||arr[1]=="error"){
		document.getElementById("BtnD"+ids).innerHTML="Delete";
		if(arr[1]!="waiting"){
			document.getElementById("BtnR"+ids).style.display="none";
			document.getElementById("BtnG"+ids).style.display="";
		}
	}
	else {
		document.getElementById("BtnD"+ids).innerHTML="&nbsp;&nbsp;Stop&nbsp;&nbsp;";
	}
	if(arr[1]=="error"){
		
	}
}

function downloadRow(ids){
	document.getElementById("zipFrame").src="zipping.do?path="+ids;
	//window.open("checkZip.do?path="+ids);
	window.parent.showDownload(ids);
}

</script>
</head>
<body>
<table class="table table-bordered"  style="width:800px;">
<thead><tr><td>Task Info</td><td>Title</td><td>Start Time</td><td  align="right" title='hours:minutes:seconds'>Time</td><td align="right">Progress</td><td><button type='button' class='btn btn-success btn-xs' onclick="window.location.reload()">Refresh All</button></td></tr></thead>
<tbody>
<%
String path = request.getScheme()+"://"+request.getHeader("host")+request.getContextPath();
String servPath = request.getSession().getServletContext().getRealPath("/");
java.text.DateFormat formattor = new java.text.SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
String[] fileName = (String[])request.getAttribute("files");
if(fileName != null){
	for(int i=0;i<fileName.length;i++){
		out.println("<tr><td width='10%' valign='top'>"+fileName[i]+"</td><td width='30%' valign='top'>");
		Argument arg ;
		try{
			arg = Argument.getArgument(servPath+"result/"+fileName[i]+"/", "arguments.xml");
		}
		catch (Exception e){
			arg = null;
		}
		File taskEndFile = new File(servPath+"result/"+fileName[i]+"/log.txt");
		double per=0.00;
		String status="waiting";
		if(taskEndFile.exists()){
			per=0.01;
			FileReader reader = null;
			BufferedReader br = null;
			//read file 
		    try {
				reader = new FileReader(servPath + "result/" + fileName[i] + "/log.txt");
				br = new BufferedReader(reader);
				String str = null;
				String line1 = "";
				String line2 = "";
				String line3 = "";
				while ((str = br.readLine()) != null) {
					if(str.contains("Exectutive directory is")){
						per=0.01;
					}
					
					if(str.contains("Zero ptm searching is processing")){
						String temp = str.replace("Zero ptm searching is processing ", "");
						temp = temp.replace(" spectra.", "");
						String[] array = temp.split("of");
						int a=Integer.parseInt(array[0].replace(" ", ""));
						int b=Integer.parseInt(array[1].replace(" ", ""));
						per=0.01+a*1500/b/100.0;
					}
					if(str.contains("Fast filtering block") && str.contains("is processing")){
						//Fast filtering block 1 is processing 2987 of 3474 spectra.
						String temp = str.replace("Fast filtering block", "");
						temp=temp.substring(temp.indexOf("is processing "),temp.length());
						temp = temp.replace("is processing ", "");
						temp = temp.replace(" spectra.", "");
						String[] array = temp.split("of");
						int a=Integer.parseInt(array[0].replace(" ", ""));
						int b=Integer.parseInt(array[1].replace(" ", ""));
						per=15+a*3000/b/100.0;
					}
					if(str.contains("Ptm searching is processing")){
						String temp = str.replace("Ptm searching is processing ", "");
						temp = temp.replace(" spectra.", "");
						String[] array = temp.split("of");
						int a=Integer.parseInt(array[0].replace(" ", ""));
						int b=Integer.parseInt(array[1].replace(" ", ""));
						per=45+a*3000/b/100.0;
					}
					if(str.contains("E-value computation is processing")){
						String temp = str.replace("E-value computation is processing ", "");
						temp = temp.replace(" spectra.", "");
						String[] array = temp.split("of");
						int a=Integer.parseInt(array[0].replace(" ", ""));
						int b=Integer.parseInt(array[1].split("scan number")[0].replace(" ", ""));
						per=75+a*2200/b/100.0;
					}
					if(str.contains("Top selector")){
						per=97;
					}
					if(str.contains("Table output")){
						per=98;
					}
					if(str.contains("XalanTransformer")){
						per=99;
					}
					if(str.contains("proteins.xml")||str.contains("Identification End!")){
						per=100;
					}
					if(str.contains("[Force stopp]")){
						status="force stop";
					}
					if(str.contains("[Exception]")){
						status="error";
					}
				}
				if(per==100){
					status="finished";
				}
				else if(RunningInfo.taskName.contains(fileName[i]) && status.equalsIgnoreCase("waiting")){
					status="running";
				}
				else {
					if(!status.equalsIgnoreCase("force stop")){
						status="error";
					}
				}
				if(status.equalsIgnoreCase("error") && arg.getRealEnd()==0){
					status="unexpected stop";
					arg.setEnd(arg.getStart());
				}
				
			} catch (IOException e) {
				e.printStackTrace();
			} finally {
				if (reader != null)
					try {
						reader.close();
					} catch (IOException e) {
						e.printStackTrace();
					}
				if (br != null)
					try {
						br.close();
					} catch (IOException e) {
						e.printStackTrace();
					}
			}
		} 
		else {
			
		}
		if(status.equalsIgnoreCase("finished")){
			if(null!=arg){
			    String foldername = arg.getSpectrum_File().replace(".msalign", "");
			    out.println("<a href='"+path+"/result/"+fileName[i]+"/"+(arg==null?"#":foldername)+"_html/proteins.html' target='_blank'>");
			}
		}
		else if(status.equalsIgnoreCase("waiting")){
			out.println("<a href='"+path+"/fileList.do?path=/"+fileName[i]+"' target='_blank'>");
		}
		else {
			//out.println("<a href='"+path+"/result/"+fileName[i]+"/log.txt' target='_blank'>");
			out.println("<a href='"+path+"/fileList.do?path=/"+fileName[i]+"' target='_blank'>");
		}
		
		if(status.equalsIgnoreCase("error")){
			out.println("<font color='red'>"+(arg==null?"File Error":arg.getTitle())+"<span id='P"+fileName[i]+"'>("+status+")</span></font>");
		}
		else{
			out.println(""+(arg==null?"File Error":arg.getTitle())+"<span id='P"+fileName[i]+"'>("+status+")</span>");
		}
		
		out.println("</a><br/><div style='display:none'>aaa</div>");
		out.println("</td><td width='20%'>"+formattor.format(new Date(arg==null?0:arg.getStart()))+"</td>");
		if(status.equalsIgnoreCase("waiting")){
			out.println("<td id='TDC"+fileName[i]+"' widht='12%' align='right' title='"+"0 hour 0 minute 0 second"+"'>"+"0:00:00"+"</td>");
		}
		else{
			out.println("<td id='TDC"+fileName[i]+"' widht='12%' align='right' title='"+(arg==null?"0 hour 0 minute 0 second":arg.getCostComment())+"'>"+(arg==null?"0:00:00":arg.getCostString())+"</td>");
		}
		out.println("<td id='TD"+fileName[i]+"' width='10%' align='right'>");
		String per_str=per+"";
		if(per_str.length()==per_str.indexOf(".")+2){
			per_str+="0";
		}
		if(per_str.length()>per_str.indexOf(".")+2){
			per_str=per_str.substring(0,per_str.indexOf(".")+3);
		}
		out.println(per_str);
		out.println("%</td><td width='18%' valign='top'>");
		if(status.equalsIgnoreCase("running") || status.equalsIgnoreCase("waiting")){
			out.println("<button type='button' id='BtnR"+fileName[i]+"' class='btn btn-success btn-xs' onclick=refreshRow('"+fileName[i]+"')>&nbsp;&nbsp;Refresh&nbsp;&nbsp;</button>");
			//out.println("<a href='result/"+fileName[i]+"/"+fileName[i]+".zip'><button type='button' id='BtnG"+fileName[i]+"' class='btn btn-primary btn-xs' style='display:none' onclick=downloadRow('"+fileName[i]+"')>Download</button></a>");
			out.println("<button type='button' id='BtnG"+fileName[i]+"' class='btn btn-primary btn-xs' style='display:none' onclick=downloadRow('"+fileName[i]+"')>Download</button>");
			if(status.equalsIgnoreCase("waiting")){
			    out.println("<button type='button' id='BtnD"+fileName[i]+"' class='btn btn-warning btn-xs' onclick=toDeleteResult('"+fileName[i]+"',this)>Delete</button>");
			}
			else{
				out.println("<button type='button' id='BtnD"+fileName[i]+"' class='btn btn-warning btn-xs' onclick=toDeleteResult('"+fileName[i]+"',this)>&nbsp;&nbsp;Stop&nbsp;&nbsp;</button>");
			}
		}
		else{
			out.println("<button type='button' id='BtnR"+fileName[i]+"' class='btn btn-success btn-xs' style='display:none' onclick=refreshRow('"+fileName[i]+"')>&nbsp;&nbsp;Refresh&nbsp;&nbsp;</button>");
			//out.println("<a href='result/"+fileName[i]+"/"+fileName[i]+".zip'><button type='button' id='BtnG"+fileName[i]+"' class='btn btn-primary btn-xs' onclick=downloadRow('"+fileName[i]+"')>Download</button></a>");
			out.println("<button type='button' id='BtnG"+fileName[i]+"' class='btn btn-primary btn-xs'  onclick=downloadRow('"+fileName[i]+"')>Download</button>");
			out.println("<button type='button' id='BtnD"+fileName[i]+"' class='btn btn-warning btn-xs'  onclick=toDeleteResult('"+fileName[i]+"',this)>Delete</button>");
		}
		out.println("</td></tr>");
	}
}
%>
</tbody>
</table>
<form id="deleteFrm" action="deleteTask.do" method="post">
<input type="hidden" name="delName" id="delName_id"/>
<input type="hidden" name="isRunning" id="runningStats"/>
</form>
<div style="display:none">
<iframe id="zipFrame" name="zipFrame"></iframe>
</div>
</body>
</html>