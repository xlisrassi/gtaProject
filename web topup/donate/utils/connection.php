<?php
require_once('configs.php');

class DB {
    public $conn;
    private $server;
    private $username;
    private $password;
    private $database;

    public function __construct($server = MYSQL_SERVER, $username = MYSQL_USERNAME, $password = MYSQL_PASSWORD, $database = MYSQL_DATABASE) {
        $this->server   = $server;
        $this->username = $username;
        $this->password = $password;
        $this->database = $database;
        $this->connect();
    }
    
    public function connect() {
        $this->conn = @new mysqli($this->server, $this->username, $this->password, $this->database);

        if ($this->conn->connect_error) {
            die("Connection failed: " . $this->conn->connect_error);
        } 
    }
    
    public function query($sql) {
        return mysqli_query($this->conn, $sql);
    }

    public function fetch($sql) {
        $query = $this->query($sql);
        if($query) {
            $data = [];
            while($r = mysqli_fetch_assoc($query)) {
                $data[] = $r;
            }
            return $data;
        }
        return [];
    }
    
    public function count($sql) {
        $query = $this->query($sql);
        return $query ? mysqli_fetch_row($query)[0] : 0;
    }
}

