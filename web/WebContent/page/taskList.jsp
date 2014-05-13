<%@ page language="java" contentType="text/html; charset=ISO-8859-1"
    import="java.io.*"
    import="edu.iupui.iac.msalignplus.dao.*"
    pageEncoding="ISO-8859-1"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
<title>Insert title here</title>
<link rel="stylesheet" href="http://cdn.bootcss.com/twitter-bootstrap/3.0.3/css/bootstrap.min.css">
<script src="http://cdn.bootcss.com/jquery/1.10.2/jquery.min.js"></script>
<script src="http://cdn.bootcss.com/twitter-bootstrap/3.0.3/js/bootstrap.min.js"></script>
<script type="text/javascript">
function toDeleteResult(name){
    document.getElementById('delName_id').value=name;
	document.getElementById('deleteFrm').submit();
}
function refreshRow(ids){
	document.getElementById("TD"+ids).innerHTML="-";
	htmlobj=$.ajax({url:"runningPercent.do?name="+ids,async:false});
	var arr = htmlobj.responseText.split(',');
	document.getElementById("TD"+ids).innerHTML=arr[0];
	document.getElementById("P"+ids).innerHTML="("+arr[1]+")";
}

function downloadRow(ids){
	window.open("zipdown.do?path="+ids);
}

</script>
</head>
<body>
<table class="table table-bordered"  style="width:800px;">
<thead><tr><td>Task Info</td><td>Title</td><td>Complete</td><td><button type='button' class='btn btn-success btn-xs' onclick="window.location.reload()">Refresh All</button></td></tr></thead>
<tbody>
<%
String path = request.getScheme()+"://"+request.getHeader("host")+request.getContextPath();
String servPath = request.getSession().getServletContext().getRealPath("/");
String[] fileName = (String[])request.getAttribute("files");
if(fileName != null){
	for(int i=0;i<fileName.length;i++){
		out.println("<tr><td width='10%' valign='top'>"+fileName[i]+"</td><td width='62%' valign='top'>");
		Argument arg = Argument.getArgument(servPath+"result/"+fileName[i]+"/", "arguments.xml");
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
					line3 = line2;
					line2 = line1;
					line1 = str;
					if(str.contains("Exectutive directory is")){
						per=1.0;
					}
					
					if(str.contains("Zero ptm search complete")){
						int a=Integer.parseInt(str.substring(str.length()-6, str.length()-5));
						int b=Integer.parseInt(str.substring(str.length()-1, str.length()));
						per=a*15/b;
					}
					if(str.contains("Fast filter")){
						per=15;
					}
					
					if(str.contains("Ptm alignment")){
						per=45;
					}
					if(str.contains("Ptm search complete")){
						int a=Integer.parseInt(str.substring(str.length()-6, str.length()-5));
						int b=Integer.parseInt(str.substring(str.length()-1, str.length()));
						per=45+a*35/b;
					}
					if(str.contains("Combine prsms")){
						per=80;
					}
					if(str.contains("Evalue computation complete")){
						String temp = str.replace("Evalue computation complete ", "");
						//System.out.println(temp.substring(0, 1));
						//System.out.println(temp.substring(5, 6));
						int a=Integer.parseInt(temp.substring(0, 1));
						int b=Integer.parseInt(temp.substring(5, 6));
						per=80+a*17/b;
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
					if(str.contains("proteins.xml")){
						per=100;
					}
				}
				if(per==100){
					status="finished";
				}
				else if(RunningInfo.taskName.contains(fileName[i])){
					status="running";
				}
				else {
					status="error";
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
			out.println("<a href='"+path+"/result/"+fileName[i]+"/spectra_html/proteins.html' target='_blank'>");
		}
		else if(status.equalsIgnoreCase("waiting")){
			out.println("<a href='"+path+"/fileList.do?path=/"+fileName[i]+"' target='_blank'>");
		}
		else {
			out.println("<a href='"+path+"/result/"+fileName[i]+"/log.txt' target='_blank'>");
		}
		
		if(status.equalsIgnoreCase("error")){
			out.println("<font color='red'>"+arg.getTitle()+"<span id='P"+fileName[i]+"'>("+status+")</span></font>");
		}
		else{
			out.println(""+arg.getTitle()+"<span id='P"+fileName[i]+"'>("+status+")</span>");
		}
		
		out.println("</a><br/><div style='display:none'>aaa</div>");
		out.println("</td><td id='TD"+fileName[i]+"' width='10%'>");
		out.println(per);
		out.println("%</td><td width='18%' valign='top'>");
		if(status.equalsIgnoreCase("running") || status.equalsIgnoreCase("waiting")){
			out.println("<button type='button' class='btn btn-success btn-xs' onclick=refreshRow('"+fileName[i]+"')>Refresh</button>");
		}
		else{
			out.println("<button type='button' class='btn btn-primary btn-xs' onclick=downloadRow('"+fileName[i]+"')>Download</button>");
		}
		out.println("<button type='button' class='btn btn-warning btn-xs' onclick=toDeleteResult('"+fileName[i]+"')>Delete</button></td></tr>");
	}
}
%>
</tbody>
</table>
<form id="deleteFrm" action="deleteTask.do" method="post">
<input type="hidden" name="delName" id="delName_id"/>
</form>
</body>
</html>