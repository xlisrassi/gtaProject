<?php
    require_once('utils/functions.php');

    $transaction_id = $_GET['transaction_id'];
    $password = $_GET['password'];
    $amount = floor($_GET['real_amount']);
    $status = $_GET['status'];

    $db = new DB();
    $db->query("UPDATE `topups` SET status='{$status}' WHERE `truemoney`='{$password}'"); // Update Status
    if( $status == 1 )
    {
        $db->query("UPDATE `topups` SET transaction_id='{$transaction_id}',amount={$amount} WHERE truemoney='{$password}'"); // Update Transaction
        
        $name = $db->fetch("SELECT name FROM `topups` WHERE truemoney='{$password}' LIMIT 1")['name'];
        $point = $points[$amount];
        $db->query("UPDATE `players` SET point=point+{$point} WHERE name='{$name}'");
        die('SUCCEED|TOPPED_UP_THB_' . $amount . '_TO_' . $name);
    }
    else
    {
        /* ไม่สามารถเติมเงินได ้ */
        die('ERROR|ANY_REASONS');
    }
?>