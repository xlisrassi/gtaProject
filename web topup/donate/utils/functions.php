<?php
require_once('configs.php');
require_once('connection.php');

function truemoneyTopup($name, $truemoney_password)
{
    if(checkName($name) == 1) {
        $apiBaseUrl = 'http://www.tmpay.net/TPG/backend.php?';
        $params = [
            'merchant_id' => MERCHANT_ID,
            'password'    => $truemoney_password,
            'resp_url'    => RESP_URL,
        ];
        $queryString = http_build_query($params);
        
        $curl_content = file_get_contents($apiBaseUrl.$queryString);

        if(strpos($curl_content,'SUCCEED') !== FALSE) {
            $name = mysqli_real_escape_string($name);
            $truemoney_password = mysqli_real_escape_string($truemoney_password);
            $ip = getenv('REMOTE_ADDR');
            
            $db = new DB();
            $db->query("INSERT INTO `topups` (name, truemoney, ip) VALUES ('{$name}', '{$truemoney_password}', '{$ip}')");
            return ['status' => true, 'message' => '<strong>เติมเงินสำเร็จแล้ว !</strong> กรุณารอระบบตรวจสอบผลการเติมเงินสักครู่ (1-5 นาที)'];
        } else {
            return ['status' => false, 'message' => '<strong>ผิดพลาด !</strong> ขออภัย, ไม่สามารถเติมเงินได้ในขณะนี้ กรุณาติดต่อทีมงาน'];
        }
    } else {
        return ['status' => false, 'message' => 'ไม่พบชื่อ <strong>'.$name.'</strong> ในเกม'];
    }
}

function checkName($name) {
    $db = new DB();
    return $db->count("SELECT COUNT(*) FROM `players` WHERE `Name` LIKE '".mysqli_real_escape_string($db->conn, $name)."'");
}

function getHistories() {
    $db = new DB();
    return $db->fetch("SELECT * FROM `topups` ORDER BY `id` DESC LIMIT 5");
}