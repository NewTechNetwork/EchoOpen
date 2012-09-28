<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


$HTMLform_start = t(<<<END
<HTML>
<HEAD>
<TITLE>PeBL</TITLE>
<META http-equiv=Content-Type content="text/html; charset=utf-8">

<style type="text/css">
<!--
BODY {
	PADDING-RIGHT: 0px; PADDING-LEFT: 0px; FONT-SIZE: 11px; PADDING-BOTTOM: 0px; MARGIN: 0px; PADDING-TOP: 0px; FONT-FAMILY: Verdana, Arial, Helvetica, sans-serif; BACKGROUND-COLOR: #ffffff; ephox-visible: false
}
TABLE {
	FONT-SIZE: 11px; FONT-FAMILY: Verdana, Arial, Helvetica, sans-serif; ephox-visible: false
}
INPUT {
	FONT-SIZE: 11px; FONT-FAMILY: Verdana, Arial, Helvetica, sans-serif; ephox-visible: false
}
TEXTAREA {
	FONT-SIZE: 11px; FONT-FAMILY: Verdana, Arial, Helvetica, sans-serif; ephox-visible: false
}
BUTTON {
	FONT-SIZE: 11px; FONT-FAMILY: Verdana, Arial, Helvetica, sans-serif; ephox-visible: false
}
SELECT {
	FONT-SIZE: 11px; FONT-FAMILY: Verdana, Arial, Helvetica, sans-serif; ephox-visible: false
}
DIV {
	FONT-FAMILY: Verdana, Arial, Helvetica, sans-serif; ephox-visible: false
}
SPAN {
	FONT-FAMILY: Verdana, Arial, Helvetica, sans-serif; ephox-visible: false
}
P {
	FONT-FAMILY: Verdana, Arial, Helvetica, sans-serif; ephox-visible: false
}
UL {
	FONT-FAMILY: Verdana, Arial, Helvetica, sans-serif; ephox-visible: false
}
LI {
	FONT-FAMILY: Verdana, Arial, Helvetica, sans-serif; ephox-visible: false
}
TD {
	FONT-FAMILY: Verdana, Arial, Helvetica, sans-serif; ephox-visible: false
}
TH {
	FONT-FAMILY: Verdana, Arial, Helvetica, sans-serif; ephox-visible: false
}
PRE {
	FONT-SIZE: 12px; FONT-FAMILY: monospace; ephox-visible: false
}
A {
	COLOR: #000000; ephox-visible: false
}
.wpsLink {
	COLOR: #000000; ephox-visible: false
}
A:active {
	COLOR: #000000; ephox-visible: false
}
.wpsLink:active {
	COLOR: #000000; ephox-visible: false
}
A:visited {
	COLOR: #665142; ephox-visible: false
}
.wpsLink:visited {
	COLOR: #665142; ephox-visible: false
}
A:hover {
	COLOR: #d94800; ephox-visible: false
}
.wpsLink:hover {
	COLOR: #d94800; ephox-visible: false
}
H1 {
	FONT-SIZE: 12px; FONT-FAMILY: Verdana, Arial, Helvetica, sans-serif; ephox-visible: false
}
.wpsButtonText {
	BORDER-RIGHT: #804a3d 1px solid; PADDING-RIGHT: 4px; BACKGROUND-POSITION: left center; BORDER-TOP: #804a3d 1px solid; PADDING-LEFT: 4px; FONT-WEIGHT: bold; PADDING-BOTTOM: 2px; MARGIN: 0px; BORDER-LEFT: #804a3d 1px solid; COLOR: #4d342e; PADDING-TOP: 2px; BORDER-BOTTOM: #804a3d 1px solid; BACKGROUND-REPEAT: repeat-x; BACKGROUND-COLOR: #dbd3c5; TEXT-ALIGN: center;
}
.wpsEditText {
	COLOR: #333333; ephox-visible: false
}
.style1 {
	font-size: 24px;
	font-family: Georgia, "Times New Roman", Times, serif;
	font-weight: bold;
	height: 40px;
	vertical-align: top;
}
.style2 {
	font-family: Arial, Helvetica, sans-serif;
	font-size: 14px;
	font-weight: normal;
}
-->
</style>
</HEAD>

<BODY marginheight="0" marginwidth="0">

<TABLE height="100%" cellSpacing=0 cellPadding=0 width="100%" border=0>
	<TBODY>
		<TR>
			<TD colSpan=2>&nbsp;</TD>
	  </TR>
		<TR>
			<TD colSpan=2>
                        <DIV id=PageBar style="DISPLAY: none">
			<TABLE cellSpacing=0 cellPadding=0 width="100%" border=1>
				<TBODY>
					<TR>
						<TD class=wpsPageBar vAlign=top noWrap align=left,>
            <TABLE cellSpacing=0 cellPadding=0 border=0>
              <TBODY>
              <TR></TR></TBODY></TABLE></TD></TR>
				</TBODY>
			</TABLE>
			</DIV>
			</TD>
		</TR>
		<TR>
			<TD width=0% height="100%"></TD>
			<TD vAlign=top width="100%" height="100%"><A name=wpsMainContent></A>
			<TABLE cellSpacing=0 cellPadding=0 width="100%" align=center border=0>
				<TBODY>
					<TR height="100%">
						<TD vAlign=top>
						<TABLE cellSpacing=0 cellPadding=0 width="100%" align=center
							border=0>
							<TBODY>
								<TR>
									<TD vAlign=top width="100%"><A
										name=7_UNK5OKRRUU88802BPRPMHG2005></A>
									<DIV class=wpsPortletBody>
									<!-- LGV in Login -->
                                                                        <SCRIPT>
                                                                                if (top.location.href != self.location.href) {
                                                                                        top.location = self.location;
                                                                                }
                                                                        </SCRIPT>
END
);

$HTMLform_end = t(<<<END
									</DIV>
									</TD>
								</TR>
							</TBODY>
						</TABLE>
						</TD>
					</TR>
				</TBODY>
			</TABLE>
			</TD>
		</TR>
	</TBODY>
</TABLE>

<SCRIPT type=text/javascript>
	var gaJsHost = (("https:" == document.location.protocol) ? "https://ssl."
			: "http://www.");
	document
			.write(unescape("%3Cscript src='"
					+ gaJsHost
					+ "google-analytics.com/ga.js' type='text/javascript'%3E%3C/script%3E"));
</SCRIPT>

<SCRIPT type=text/javascript>
	var pageTracker = _gat._getTracker("UA-5818281-61");
	pageTracker._trackPageview();
</SCRIPT>

</BODY>
</HTML>

END
);

$HTMLsigned_out = t(<<<END
    <TABLE
            style="BORDER-RIGHT: black 3px solid; BORDER-TOP: black 3px solid; BORDER-LEFT: black 3px solid; BORDER-BOTTOM: black 3px solid"
            cellSpacing=0 cellPadding=2 width="50%" align=center border=0>
            <TBODY>
                    <TR>
                            <TD width="38%" bgcolor="#FFFFFF">
                            <TABLE width="100%" align=left border=0>
                                    <TBODY>
                                <TR>
                                  <TD align=left class="style1"><div align="center" class="style1">Google Apps Login</div></TD>
                                </TR>
                                <TR>
                                  <TD class=wpsEditText><div align="center" class="style2">You have been successfully signed out from Google Apps</div></TD>
                              </TR>
                                <TR>
                                        <TD class=wpsEditText align=left><div align="center"></div></TD>
                                </TR>
                                <TR>
                                        <TD><div align="center"></div></TD>
                                </TR>
                                    <TR>
                                        <TD>
                                          <div align="center">
                                            <TABLE cellSpacing=4 cellPadding=0 border=0>
                                              <TBODY>
                                                <TR>
                                                  <TD noWrap>
                                                    <a class=wpsButtonText style="CURSOR: pointer" type=submit align=absMiddle
                                                        href="https://mail.google.com/a/@domainName" >
                                                        Click here to sign in again to Google Apps</a>
                                                  </TD>
                                            </TR>
                                          </TBODY>
                                      </TABLE>
                                </div></TD>
                                </TR>
                                <TR>
                                <TD>
                                  <div align="center">

                                </div></TD>
                                </TR>
                        </TBODY>
                      </TABLE>
                    </TD>
                </TR>
            </TBODY>
    </TABLE>
END
);

$HTMLsignin_namep = t(<<<END
    <BR>
    <BR>
    <TABLE style="BORDER-RIGHT: black 3px solid; BORDER-TOP: black 3px solid; BORDER-LEFT: black 3px solid; BORDER-BOTTOM: black 3px solid"
        cellSpacing=0 cellPadding=2 width="50%" align=center border=0>
        <TBODY>
            <TR>
                <TD width="38%" bgcolor="#FFFFFF">
                <TABLE width="100%" align=left border=0>
                <TBODY>
                    <TR>
                      <TD align=left class="wpsEditText style1"><div align="center" class="style1">Google Apps Login</div></TD>
                    </TR>
                    <TR>
                      <TD class=wpsEditText><div align="center" class="style2">Please enter your PeBL credentials below to login to your mail</div></TD>
                    </TR>
                    <TR>
                        <TD class=wpsEditText align=left><div align="center"></div></TD>
                    </TR>
                    <TR>
                        <TD><div align="center">
                          <table border="0" align="center" cellpadding="2">
                            <tr valign="middle">
                            <TD align=left class=wpsEditText><LABEL for=userID>
                            <div align="right"><B>E-mail Address:</B></div>
                            </LABEL></TD>
                            <TD align=left class=wpsFieldText>
END
);
$HTMLsignin_pwdp = t(<<<END
                            </td>
                            </tr>
                            <tr>
                            <TD align=left valign="middle" class=wpsEditText><LABEL for=password>
                            <div align="right"><B>Password:</B></div>
                            </LABEL></td>
                            <TD align=left valign="middle" class=wpsFieldText>
END
);
$HTMLsignin_pwds = t(<<<END
                            </TD>
                            </tr>
                          </table>
                          </div></TD>
                            </TR>
                            <TR>
                                    <TD><div align="center"></div></TD>
                            </TR>
                            <TR>
                                    <TD>
                                      <div align="center">
                                        <TABLE cellSpacing=4 cellPadding=0 border=0>
                                          <TBODY>
                                            <TR>
                                              <TD noWrap>
END
);
$HTMLsignin_submits = t(<<<END
                                              </TD>
                                              <TD noWrap>
END
);
$HTMLsignin_cancels = t(<<<END
                                              </TD>
                                          </TR>
                                        </TBODY>
                                      </TABLE>
                            </div></TD>
                            </TR>
                    </TBODY>
            </TABLE>
            </TD>
            </TR>
        </TBODY>
    </TABLE>
    <BR>
    <BR>

    <SCRIPT language=JavaScript>
            function setCancelURL() {
                var txtEdit = document.getElementById('edit-name');
                var txtPwd = document.getElementById('edit-pass');
                txtEdit.value = '';
                txtPwd.value = '';
            }
            function setFocus() {
                var txtEdit = document.getElementById('edit-name');
                txtEdit.focus();
            }
            setTimeout(setFocus, 400);
    </SCRIPT>
    </P>
END
);

$HTMLsignout = t(<<<END
    <TABLE style="BORDER-RIGHT: black 3px solid; BORDER-TOP: black 3px solid; BORDER-LEFT: black 3px solid; BORDER-BOTTOM: black 3px solid"
            cellSpacing=0 cellPadding=2 width="50%" align=center border=0>
        <TBODY>
            <TR>
                <TD width="38%" bgcolor="#FFFFFF">
                    <TABLE width="100%" align=left border=0>
                    <TBODY>
                    <TR>
                      <TD align=left class="wpsEditText style1"><div align="center" class="style1">Google Apps Login</div></TD>
                    </TR>
                    <TR>
                      <TD class=wpsEditText><div align="center" class="style2">You have been successfully signed out from Google Apps</div></TD>
                    </TR>
                    <TR>
                      <TD class=wpsEditText align=left><div align="center"></div></TD>
                    </TR>
                    <TR>
                      <TD><div align="center"></div></TD>
                    </TR>
                    <TR>
                      <TD>
                          <div align="center">
                            <TABLE cellSpacing=4 cellPadding=0 border=0>
                              <TBODY>
                                <TR>
                                  <TD noWrap><a class=wpsButtonText
                                        style="CURSOR: pointer" type=submit align=absMiddle
                                        href="https://docs.google.com/a/@domainName" >
                                                Click here to sign in again to Google Apps</a>
                                  </TD>
                                </TR>
                              </TBODY>
                            </TABLE>
                            </div>
                        </TD>
                    </TR>
                    </TBODY>
              </TABLE>
            </TD>
        </TR>
    </TBODY>
    </TABLE>

END
);

?>
