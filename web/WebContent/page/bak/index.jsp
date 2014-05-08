<!DOCTYPE html>
<html>
<head>
<meta charset="ISO-8859-1">
<title>Insert title here</title>
</head>
<body>

<form action="run.do" method="post" target="resultFrame">
<input type="hidden" name="command" value="clean"/>
	<input type="submit" value="clean" />
</form>

<a href="toSpectraFile.do">spectra file upload</a>
<br/>
<a href="toDatabaseFile.do">database file upload</a>
<br/>
<a href="toConfig.do">config files upload</a>

<table><tr><td>
<form action="run.do" method="post" target="resultFrame">
<input type="hidden" name="command" value="run"/>
	<input type="submit" value="RunApp" />
</form>
</td><td>
<form action="fileList.do" method="post" target="resultFrame">
<input type="hidden" name="path" value="in"/>
	<input type="submit" value="show result" />
</form>
</td></tr></table>
<table><tr><td>
<form action="run.do" method="post" target="resultFrame">
<input type="hidden" name="command" value="ht"/>
	<input type="submit" value="GeneHTML" />
</form>
</td><td>
<form action="fileList.do" method="post" target="resultFrame">
<input type="hidden" name="path" value="html"/>
	<input type="submit" value="show html" />
</form>
</td></tr></table>

<form action="run.do" method="post"  target="resultFrame">
<input type="input" name = "command" value="tasklist"/>
	<input type="submit" value="RUN" />
</form>

<iframe name="resultFrame"  width="500" height="300"> 

</iframe> 

</body>
</html>