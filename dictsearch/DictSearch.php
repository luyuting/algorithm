<?php
/**
 * File Name:    DictSearch.php
 * Author:       luyuting
 * Email:        luyuting415@mail.dlut.edu.cn
 * Created Time: Fri 03 Feb 2017 02:07:25 PM CST
 **/
class Node {

    public $index = -1;

    public $trace = null;

    public $child = [];
}

class DictSearch {

    private $_root = null;

    private $_keys = [];

    public function __construct(array $keys) {
        $this->_root = new Node();
        $this->_keys = $keys;

        foreach ($keys as $index => $key){
            $this->insert($key, $index);
        }
        $this->setTrace();
    }

    private function setTrace() {
        $que = new SplQueue();
        $que->enqueue($this->_root);

        while (!$que->isEmpty()) {
            $node = $que->dequeue();

            foreach ($node->child as $c => $child) {
                $tmp = $node;
                while (true) {
                    $tmp = $tmp->trace;
                    if ($tmp == null) {
                        $child->trace = $this->_root;
                        break;
                    } else if (isset($tmp->child[$c])) {
                        $child->trace = $tmp->child[$c];
                        break;
                    }
                }

                $que->enqueue($child);
            }
        }
    }

    private function insert($key, $index) {
        $node = $this->_root;
        $len = strlen($key);

        for ($i = 0; $i < $len; $i ++) {
            $c = $key{$i};
            if (!isset($node->child[$c])) {
                $node->child[$c] = new Node();
            }
            $node = $node->child[$c]; 
        }
        $node->index = $index;
    }

    public function search($word) {
        $node = $this->_root;
        $indexsMap = [];
        $mappedKeys = [];
        $len = strlen($word);

        for ($i = 0; $i < $len; $i ++) {
            $c = $word{$i};

            if (isset($node->child[$c])) {
                $node = $node->child[$c];
            } else {

                while (true) {
                    $node = $node->trace;
                    if ($node == null) {
                        $node = $this->_root;
                        break;
                    } else if (isset($node->child[$c])) {
                        $node = $node->child[$c];
                        break;
                    }
                }
            }

            if ($node->index != -1) {
                $indexsMap[$node->index] = true;
                $tmp = $node->trace;

                while ($tmp) {
                    if ($tmp->index != -1) {
                        $indexsMap[$tmp->index] = true;
                    }
                    $tmp = $tmp->trace;
                }
            }
        }

        foreach ($indexsMap as $k => $v) {
            $mappedKeys[] = $this->_keys[$k];
        }

        return $mappedKeys;
    }
}
