<!DOCTYPE html>
<html>
  <head>
    <title>TopID</title>
    <link rel="stylesheet" href="http://cdn.bootcss.com/twitter-bootstrap/3.0.3/css/bootstrap.min.css">
    
  </head>
  <body>
    <script src="http://cdn.bootcss.com/jquery/1.10.2/jquery.min.js"></script>
    <script src="http://cdn.bootcss.com/twitter-bootstrap/3.0.3/js/bootstrap.min.js"></script>

	<script type="text/javascript">
	var msalign_check_id;
	var cur_process = 0;
	function submitArgument(){
		
		
		if(document.getElementById('taskTitle').value==""){
			alert("Title can not be empty!");
			return;
		}
		if(document.getElementById('specPath').value==""){
			alert("Spectra file can not be empty!");
			return;
		}
		if(document.getElementById('dataPath').value==""){
			alert("Database file can not be empty!");
			return;
		}
		
		var arr = document.getElementById('specPath').value;
		if(arr.length<=8 || arr.substr(arr.length-8,8)!=".msalign"){
			alert("Spectra file must be msalign file!");
			return;
		}

		arr = document.getElementById('dataPath').value;
		if(arr.length<=6 || arr.substr(arr.length-6,6)!=".fasta"){
			alert("Database file must be fasta file!");
			return;
		}
 		var formobject = document.getElementById("arguments");
 		formobject.submit();
 		$("#myModal").modal({show:true});
		
	}
	
	function showDownload(ids){
		document.getElementById("downloadFrm").src="checkZip.do?path="+ids;
		$("#downloadModal").modal({show:true});
	}
	
	function refreshProcessingBar(){
		htmlobj=$.ajax({url:"runninginfo.do",async:false});
        //lert(htmlobj.responseText);
        document.getElementById("runInfo").innerHTML=htmlobj.responseText;
		document.getElementById("runningbar").style.width=""+cur_process+"%";
		if(htmlobj.responseText.indexOf("create folder xml/species")!=-1){
			document.getElementById("runningbar").style.width="100%";
			window.clearTimeout(msalign_check_id); 
			document.getElementById("resultFrame").src="./folderList.do";
			document.getElementById("resultTab").click();
		}
		cur_process+=5;
		if(cur_process>=105){
			window.clearTimeout(msalign_check_id); 
		}
	}
	
	function starProcessing(){
		document.getElementById("runtab").click();
		msalign_check_id = window.setInterval(refreshProcessingBar, 3000);
	}
	
	function checkInt(d,v){
		var t = d.value;
		if(t.length<=0)
		{
		  alert("can not be blank!");    
		}else
		{
		    for(var i=0;i<t.length;i++){
		    if (t.charAt(i) in ['0','1','2','3','4','5','6','7','8','9'])
		    { 
		    }else{ 
		    	alert("must be positive");
		      d.value=v;
		      d.focus();
		      break;
		    }
		    }
		}
	}
	
	function checkNum(d){
		var t = d.value;
		if(isNaN(Number(t))){
			alert("must be number!");
			d.value="0.01";
			d.focus();
		}
		
	}
	
	function showTask(){
		$('#myModal').modal("hide");
		document.getElementById("resultTab").click();
		document.getElementById('resultFrame').src="./taskList.do";
		document.getElementById('taskTitle').value='';
		document.getElementById('specPath').value=''
	}
	

	</script>

	<!-- header -->
	<div class="bs-header" id="content">
		<div class="container">
			<h1 style="color:#0099DD;">TopID</h1>
		</div>
	</div>

	<div class="container bs-docs-container">

		<!-- Nav tabs -->
		<ul class="nav nav-tabs">
			<li><a href="#msalignplus" data-toggle="tab">TopID</a></li>
<!-- 			<li><a href="#profile" data-toggle="tab" id="runtab">Running</a></li> -->
			<li><a href="#result" data-toggle="tab" id="resultTab" onclick="document.getElementById('resultFrame').src='./taskList.do';">Results</a></li>
		</ul>

		<!-- Tab panes -->
		<div class="tab-content">
			<div class="tab-pane active" id="msalignplus" style="width:900px">
			   <form id="arguments" action="process.do" method="post" enctype="multipart/form-data" target="addTask">
			   <input type="hidden" id="specTitle" name="titleh" value="tasklist" /> 
			    <br/>
				<div class="panel panel-default" style='padding: 20px 20px 20px 20px;width:860px'>
					<div class="panel-heading">Search Infomation</div>
					<div class="panel-body">
					    <div class="input-group input-group">
							<span class="input-group-addon">Title:</span> 
							<input type="text" class="form-control" id="taskTitle" name="title" onchange="document.getElementById('specTitle').value=this.value">
						</div>
						<br/>
						<div class="input-group input-group">
							<span class="input-group-addon">Spectrum File:</span> 
							<table><tr><td>
							<input type="file" id="specPath" class="form-control" name="specFile" ></td><td>
							</td></tr></table>
						</div>
						<br/>
						<div class="input-group input-group">
							<span class="input-group-addon">Database File:</span> 
							<table><tr><td>
							<input type="file" id="dataPath" name="dataFile" class="form-control" ></td><td>
							</td></tr></table>
						</div>
					</div>
				</div>

				<div class="panel panel-default" style='padding: 20px 20px 20px 20px;width:860px'>
					<div class="panel-heading">Search Arguments</div>
					<div class="panel-body">
						<div class="input-group input-group">
						    <label class="col-sm-2 control-label" style="width:250px">Fragmentation Method:</label>
							<select	class="form-control" name="fragMeth" style="width:250px" >
								<option>FILE</option>
								<option>CID</option>
								<option>ETD</option>
								<option>HCD</option>
							</select>
						</div>
						<br />
						<div class="input-group input-group">
							<label class="col-sm-2 control-label" style="width:250px">Protein N-Terminal Variable PTM:</label>
							<div class="input-group input-group">
							<input type="hidden" name="nptm1" value="NONE">
							&nbsp;&nbsp;&nbsp;&nbsp;<input type="checkbox" name= "nptm2" value ="NME" checked="true"> NME
							&nbsp;&nbsp;&nbsp;&nbsp;<input type="checkbox" name= "nptm3" value = "NME_ACETYLATION" checked="true"> NME and N-Terminal Acetylation
							</div>
						</div>
						<br />
						<div class="input-group input-group">
							<label class="col-sm-2 control-label" style="width:330px">Parent and Fragment Mass Error Tolerance:</label>
							<input type="text" class="form-control" name="error" style="width:250px" value="15"  check-type="required" onblur="checkInt(this,15)">PPM
<!-- 							<span class="input-group-addon">PPM</span>  -->
						</div>
						<br />
						<div class="input-group input-group">
							<label class="col-sm-2 control-label" style="width:250px">Cysteine Protecting Group:</label>
							<select	class="form-control" name="cpg" style="width:250px" >
								<option value="C0">NONE</option>
								<option value="C57">Carbamidomethylation (C57)</option>
								<option value="C58">Carboxymethylation (C58)</option>
							</select>
						</div>
						<br />
						<div class="input-group input-group">
							<label class="col-sm-2 control-label" style="width:250px">Decoy:</label>
							<input type="checkbox" name= "sType" value ="TARGET+DECOY"> 
						</div>
						<br />
						<div class="input-group input-group">
							<label class="col-sm-2 control-label" style="width:250px">Cutoff Type:</label>
							<select	class="form-control" name="cType" style="width:250px" >
								<option>EVALUE</option>
								<option>FDR</option>
							</select>
						</div>
						<br />
						<div class="input-group input-group">
							<label class="col-sm-2 control-label" style="width:250px">Cutoff Value:</label>
							<input type="text" class="form-control" name="cutValue" style="width:250px" value="0.01" onblur="checkNum(this)">
						</div>
						<br />
						<div class="input-group input-group">
							<label class="col-sm-2 control-label" style="width:250px">Number of Unexpected PTM:</label>
							<select	class="form-control" name="nuPTM" style="width:250px" >
								<option>2</option>
								<option>1</option>
								<option>0</option>
							</select>
						</div>
						<br />
						<div class="input-group input-group">
							<label class="col-sm-2 control-label" style="width:250px">Max PTM Mass:</label>
							<input type="text" class="form-control" name="maxptmmass" style="width:250px" value="1000000" onblur="checkInt(this,1000000)">
						</div>
						<br />
<!-- 						<input type="submit" class="btn btn-primary" value="Submit" /> -->
						<button type="button" id="submitBtn" data-loading-text="Submit..." class="btn btn-primary" onclick="submitArgument()">Submit</button>
											
<!-- 						<button type="button" class="btn btn-primary" onclick="starProcessing()">ssss</button> -->
					</div>
				</div>
				</form>
			</div>
<!-- 			result page 
			<div class="tab-pane" id="profile">
				<div class="panel panel-default" style='padding: 20px 20px 20px 20px;'>
					<div class="panel-heading">Running</div>
					<div class="progress progress-striped active">
						<div class="progress-bar" role="progressbar" aria-valuenow="45" id="runningbar"
							aria-valuemin="0" aria-valuemax="100" style="width: 0%">
						</div>
					</div>
					<label  id="runInfo" style="width:100%;height:100px">
					Number of Unexp
					ected PTM:
					</label>
					<div style="display:none"">
				    <iframe name="runningInfo" style="width:100%;height:100%"></iframe>
				    </div>
				</div>
			</div>-->
			<div class="tab-pane" id="result" style="width:900px">
				<div class="panel panel-default" style='padding: 20px 20px 20px 20px;width:860px'>
				<iframe id="resultFrame" name="file" src="./taskList.do" width="820px" height="400px"></iframe>
			    </div>
			</div>
		</div>

	</div>
	
<div class="modal fade" id="myModal">
  <div class="modal-dialog">
    <div class="modal-content">
      <div class="modal-header">
        <button type="button" class="close" data-dismiss="modal" aria-hidden="true">&times;</button>
        <h4 class="modal-title">Processing</h4>
      </div>
      <div class="modal-body">
        <p>Info</p>
        <div>
        <iframe id="subFrame" name="addTask" src="page/uploading.jsp" width="550px" height="150px"></iframe>
        </div>
      </div>
      <div class="modal-footer">
        <button type="button" class="btn btn-default" data-dismiss="modal" onclick="document.getElementById('taskTitle').value='';document.getElementById('specPath').value=''">Submit Another Task</button>
        <button type="button" class="btn btn-primary" onclick="showTask()">See Results</button>
      </div>
    </div><!-- /.modal-content -->
  </div><!-- /.modal-dialog -->
</div><!-- /.modal -->
	
<div class="modal fade" id="downloadModal">
  <div class="modal-dialog">
    <div class="modal-content">
      <div class="modal-header">
        <button type="button" class="close" data-dismiss="modal" aria-hidden="true">&times;</button>
        <h4 class="modal-title">Processing</h4>
      </div>
      <div class="modal-body">
        <p>Info</p>
        <div>
        <iframe id="downloadFrm" name="downloadTask" src="" width="550px" height="150px"></iframe>
        </div>
      </div>
      <div class="modal-footer">
        <button type="button" class="btn btn-default" data-dismiss="modal">Close</button>
      </div>
    </div><!-- /.modal-content -->
  </div><!-- /.modal-dialog -->
</div><!-- /.modal -->

</body>
</html>