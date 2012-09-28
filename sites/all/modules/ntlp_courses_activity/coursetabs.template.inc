<?php

/* 
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
*/


$get=$_GET['q'];
$br= explode("/",$get);
$cnt=count($br);

# Getting Page Name From URl like event,node...
$target=$br[$cnt-2];

#Getting Query String  From URL
$qs =$br[$cnt-1];

$HTMLcourse_tab =t(<<<END
 
<div class="Agenda">
<div class="AgendaMenu">
<ul>

END
);
 
if($target=="node"){

 $HTMLcourse_tab .=t(<<<END

    <li class="active">
    <a class="active" href="?q=node/$qs


END
);

 
 $HTMLcourse_tab .=t(<<<END

 ">Start</a></li>
END
);



 }  else {


  $HTMLcourse_tab .=t(<<<END

         <li>
    <a href="?q=node/$qs">Start</a></li>



END
);

  }
  
  $HTMLcourse_tab .=t(<<<END
  
          <li>


END
);


    if($target=="events"){  

 $HTMLcourse_tab .=t(<<<END
</li>
<li class="active">
    <a class="active" href="?q=nav/courses/events/$qs">Agendas</a></li>


END
);

  }  else{

   $HTMLcourse_tab .=t(<<<END
 
      <li>
    <a href="?q=nav/courses/events/$qs">Agendas</a></li>
<li>
           

END
);

}
    if($target=="activity"){

   $HTMLcourse_tab .=t(<<<END


           </li>
<li class="active">
    <a class="active" href="?q=ntlp/courses/activity/$qs">Activities</a></li>



END
);
 }  else {

$HTMLcourse_tab .=t(<<<END

           <li>
    <a href="?q=ntlp/courses/activity/$qs">Activities</a></li>


END
);


 }
    if($target=="groups"){

$HTMLcourse_tab .=t(<<<END



           <li class="active">
    <a class="active" href="?q=nav/courses/groups/$qs">Project</a></li>

END
);


  }  else {

$HTMLcourse_tab .=t(<<<END
      
           <li>
    <a href="?q=nav/courses/groups/$qs">Project</a></li>

END
);


} 
 if($target=="people"){
        
$HTMLcourse_tab .=t(<<<END
                
                
        <li class="active">
        <a class="active" href="?q=nav/courses/people/$qs">Discussions</a></li>


END
);

  
  }  else {

$HTMLcourse_tab .=t(<<<END

      <li>
    <a href="?q=nav/courses/people/$qs">Discussions</a></li>

END
);

  }

    if($target=="library"){


$HTMLcourse_tab .=t(<<<END

<li class="active">
    <a class="active" href="?q=nav/courses/library/$qs">Resources</a></li>

END
);

  }  else {

$HTMLcourse_tab .=t(<<<END
        
      <li>
    <a href="?q=nav/courses/library/$qs">Resources</a></li>
END
);
 } 
    if($target=="grade"){  
        
$HTMLcourse_tab .=t(<<<END
        <li class="active">

    <a class="active" href="?q=nav/courses/grade/$qs">Grades</a></li>

END
);

 }  else {

$HTMLcourse_tab .=t(<<<END

     <li>
    <a href="?q=nav/courses/grade/$qs">Grades</a></li>
END
);


}


$HTMLcourse_tab .=t(<<<END
</ul>
<div class="AgendaRightShade">
    &nbsp;</div>
</div>
<div class="AgendaContents">
    <table border="0" cellpadding="0" cellspacing="0" width="100%">
        <tbody>
            <tr>
                <td class="AgendaMainContentAreaMiddleLeftCorner">
                    &nbsp;</td>
                <td class="AgendaMainContentAreaMiddleCenterCorner">
                    &nbsp;</td>
                <td class="AgendaMainContentAreaMiddleRightCorner">
                    &nbsp;</td>
            </tr>
        </tbody>
    </table>
</div>
</div>
 
END
);
?>
