<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


class logfile {

    private $log_file = "/sites/default/files/log/_DATE__message.log";
    private $fp = null;

    function __construct($filename) {
        if (isset($filename)) {
            $this->log_file = $filename;
        } else {
            // define the current date (it will be appended to the log file name)
            $today = date('Y-m-d');
            $this->log_file = $_SERVER['DOCUMENT_ROOT'] . str_replace("_DATE_", $today, $this->log_file);
        }
        $this->lopen();
    }
    function  __destruct() {
        if (isset($this->fp)) {
            fclose($this->fp);
        }
    }
    // write message to the log file
    public function lwrite($message) {
//        $script_name = pathinfo($_SERVER['PHP_SELF'], PATHINFO_FILENAME);
        // define current time

        // write current time, script name and message to the log file
//        fwrite($this->fp, "--------------------------------------------------------------------------------------------------------\n$message\n");
        fwrite($this->fp, $message."\r\n");
    }

    // open log file
    private function lopen() {
        // define log file path and name
        $lfile = $this->log_file;
        // open log file for writing only; place the file pointer at the end of the file
        // if the file does not exist, attempt to create it
//        $filesize = filesize($lfile);
//        if($filesize >= 1048576) {
//         echo 'file size reached to 1 mb';
//        }

        $this->fp = fopen($lfile, 'a') or exit("Can't open $lfile!");
    }

    private function check_file() {
//
//        $directory = "/ntlplog/";
//        $filecount = count(glob("" . $_SERVER['DOCUMENT_ROOT'].$directory . "*.txt"));
//
//        if($filecount == 0) {
//            $this->fp =  fopen($_SERVER['DOCUMENT_ROOT'].$this->$log_file,'a+') or exit("Can't open $lfile!");
//
//        }else {
//            $file_name = $_SERVER['DOCUMENT_ROOT'].$this->$log_file.'_'.$file_count-1;
//            if(file_exists($file_name) ) {
//                $this->fp =  fopen($_SERVER['DOCUMENT_ROOT'].$this->$log_file.'_'.$file_count,'a+') or exit("Can't open $lfile!");
//            }
//
//        }
    }

}



?>
