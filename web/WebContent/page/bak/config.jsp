<%@ page language="java" contentType="text/html; charset=GB18030"
	pageEncoding="UTF-8"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=GB18030">
<title>config files</title>
</head>
<body>
	<h2>config files upload</h2>

	<form action="config.do" method="post"
		enctype="multipart/form-data">
		<p>
			acid:<input type="file" name="files">
		<p>
			activation:<input type="file" name="files">
		<p>
			configuration:<input type="file" name="files">
		<p>
			fix_mod_residue:<input type="file" name="files">
		<p>
			ion_type:<input type="file" name="files">
		<p>
			neutral_loss:<input type="file" name="files">
		<p>
			prot_mod:<input type="file" name="files">
		<p>
			ptm:<input type="file" name="files">
		<p>
			residue:<input type="file" name="files">
		<p>
			support_peak_type:<input type="file" name="files">
		<p>
			trunc:<input type="file" name="files">
		<p>
			<input type="submit" value="submit">
	</form>

</body>
</html>