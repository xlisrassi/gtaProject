<?php
require_once('utils/functions.php');

if(isset($_POST['name'])) {
    $result = truemoneyTopup($_POST['name'], $_POST['truemoney']);
}
$histories = getHistories()
?>
<!DOCTYPE html>
<html>
    <head>
        <meta charset="utf-8">
        <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">
        <meta name="description" content="Truemoney Topup">
        <meta name="author" content="Sorrawich Sirinararat">
        <link rel="icon" href="favicon.ico">
    
        <title>Truemoney Topup | Drag-GTA</title>
        <link rel="stylesheet" type="text/css" href="bower_components/bootstrap/dist/css/bootstrap.min.css"/>
        <link rel="stylesheet" type="text/css" href="assets/custom.css"/>
    </head>

    <body>
        <div class="container">
                <?php
                    if(isset($result)):
                        if($result['status']):
                        ?>
                            <div class="alert alert-success">
                                <?php echo $result['message']; ?>
                            </div>
                        <?php
                        else:
                        ?>
                            <div class="alert alert-danger">
                                <?php echo $result['message']; ?>
                            </div>
                        <?php
                        endif;
                ?>
                <?php
                    endif;
                ?>
            <form class="form-signin" method="POST" onSubmit="return confirm('คุณยืนยันที่จะเติมเงินใช่หรือไม่')">
                <div class="panel panel-default">
                    <div class="panel-body">
                        <div class="page-header">
                            <h3>ระบบเติมเงิน</h3>
                        </div>

                        <label for="name" class="sr-only">ชื่อในเกม</label>
                        <input type="text" id="name" name="name" class="form-control" placeholder="ชื่อในเกม" required autofocus value="<?php echo isset($_POST['name']) ? $_POST['name'] : '' ?>">

                        <label for="truemoney" class="sr-only">รหัสบัตรทรูมันนี่ 14 หลัก</label>
                        <input type="number" id="truemoney" name="truemoney" class="form-control" placeholder="รหัสบัตรทรูมันนี่ 14 หลัก" required autocomplete="off" maxlength="14" value="<?php echo isset($_POST['truemoney']) ? $_POST['truemoney'] : '' ?>">
                        
                        <button class="btn btn-lg btn-primary btn-block" type="submit">เติมเงิน</button>
                    </div>
                </div>
                
            </form>

            <div class="row">
                <div class="col-md-6">
                    <div class="panel panel-default">
                        <div class="panel-body">
                            <div class="page-header">
                                <h3>อัตราแลกเปลี่ยน</h3>
                            </div>
                            <table class="table table-stripted table-bordered">
                                <thead>
                                    <tr>
                                        <th>ราคาบัตร</th>
                                        <th>ได้รับ</th>
                                    </tr>
                                </thead>

                                <tbody>
                                    <?php
                                        foreach($points as $amount => $point):
                                    ?>
                                        <tr>
                                            <td><?php echo number_format($amount); ?></td>
                                            <td><?php echo number_format($point); ?> Points</td>
                                        </tr>
                                        
                                    <?php
                                        endforeach;
                                    ?>
                                </tbody>
                            </table>
                        </div>
                    </div>
                </div>
                <div class="col-md-6">
                    <div class="panel panel-default">
                        <div class="panel-body">
                            <div class="page-header">
                                <h3>การเติมเงินล่าสุด (5 รายการ)</h3>
                            </div>
                            <table class="table table-stripted table-bordered">
                                <thead>
                                    <tr>
                                        <th>รหัสบัตรทรูมันนี่</th>
                                        <th>สถานะ</th>
                                    </tr>
                                </thead>

                                <tbody>
                                    <?php
                                        foreach($histories as $history):
                                    ?>
                                        <tr>
                                            <td><?php echo $history['truemoney']; ?></td>
                                            <td><span class="label label-success"><span class="glyphicon glyphicon-ok"></span> ตรวจสอบแล้ว</span></td>
                                        </tr>
                                    <?php
                                        endforeach;
                                    ?>
                                </tbody>
                            </table>
                        </div>
                    </div>
                </div>
            </div>
        </div> <!-- /container -->

        <script src="bower_components/jquery/dist/jquery.min.js"></script>
        <script src="bower_components/bootstrap/dist/js/bootstrap.min.js"></script>
    </body>
</html>