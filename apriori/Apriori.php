<?php
class Apriori {
    /**
     * 最小支持度阈值
     * @var float
     */
    private $_support = 0.2;
    
    /**
     * 最小置信度阈值
     * @var float
     */
    private $_confidence = 0.5;
   
    /**
     * 数据项之间分隔符
     * @var string
     */
    private $_separation = '|';
    
    /**
     * 关联规则分隔符
     * @var string
     */
    private $_connection = '->';
    
    /**
     * 原始数据集
     * @var array
     */
    private $_data = [];
    
    /**
     * 保存唯一实例
     * @var Apriori
     */
    private static $_instance = null;
   
    /**
     * 禁止在类外通过调用构造函数实例化
     */
    private function construct() {
        
    }
    
    /**
     * 单例模式
     * @return Apriori
     */
    public static function getInstance() {
        if (is_null(self::$_instance)) {
            self::$_instance = new self();
        }
        return self::$_instance;
    }
    
    /**
     * 获得频繁项集
     * @return array
     */
    private function getFrequentCollections() {
        $frequents = [];
        $frequent = [];
        $count = count($this->_data);

        do {
            $candidate = $this->getCandidateCollections($frequent);

            $frequent = [];
            foreach ($candidate as $cand_k => $cand_v) {
                $cand_items = explode($this->_separation, $cand_k);
                foreach ($this->_data as $data) {
                    $items = explode($this->_separation, $data);
                    !array_diff($cand_items, $items) && $candidate[$cand_k] ++;
                }
                $candidate[$cand_k] /= $count;
            }
            $frequent = array_filter($candidate, function ($v) {
                return $v >= $this->_support;
            });

            if (empty($frequent)) {
                break;
            }
            $frequents[] = $frequent;            

        } while (true);

        return $frequents;
    }
    
    /**
     * 获得候选项集
     * @param array $frequent 频繁项集
     * @return array
     */
    private function getCandidateCollections(array $frequent) {
        $candidate = [];  
        $count = count($frequent);

        if ($count == 0) {
            foreach ($this->_data as $data) {
                $initial_data = explode($this->_separation, $data);
                foreach ($initial_data as $initial) {
                    !isset($candidate[$initial]) && $candidate[$initial] = 0;
                }
            }
            ksort($candidate);
            return $candidate;
        }
        
        $items = [];
        foreach ($frequent as $freq_k => $freq_v) {
            $items[] = explode($this->_separation, $freq_k);
        }

        for ($i = 0; $i < $count - 1; $i ++) {
            for ($j = $i + 1; $j < $count; $j ++) {
                $diff = array_diff($items[$i], $items[$j]);
                if (count($diff) != 1) {
                    continue;
                }
                $prepare = array_merge($items[$j], $diff);
                sort($prepare);
                $prepare_str = implode($this->_separation, $prepare);
                !isset($candidate[$prepare_str]) && $candidate[$prepare_str] = 0;
            }
        }  
        ksort($candidate);
        return $candidate;
    }
    
    /**
     * 获得真子集
     * @param array $set 集合项
     * @return array
     */
    private function getProperSubset(array $set) {
        $subset = [];
        $size = count($set);
        for ($i = 1; $i < $size; $i ++) {
            // $k_subset = [];
            $this->getKSubset($subset, $set, 0, $size, [], $i);
            // $subset[$i] = $k_subset;
        }
        return $subset;
    }
    
    /**
     * 递归获得k项真子集
     * @param array $k_subset
     * @param array $set
     * @param integer $start
     * @param integer $end
     * @param array $candidate
     * @param integer $k
     * @return void
     */
    private function getKSubset(array &$k_subset, array $set, $start, $end, array $candidate, $k) {
        if ($k == 0) {
            $k_subset[] = $candidate;
            return;
        }
        for ($i = $start; $i < $end; $i ++) {
            $copy = $candidate;
            $copy[] = $set[$i];
            $this->getKSubset($k_subset, $set, $i + 1, $end, $copy, $k - 1);
        }
    }
    
    /**
     * 非递归获得真子集
     * @param array $set
     * @return array
     */
    private function getSubset(array $set) {
        $pre_subset = [];
        $k_subset = [];

        foreach ($set as $data_k => $data_v) {
            $k_subset[] = [
                'next' => ++ $data_k,
                'value' => [$data_v]
            ];
        }

        $pre_subset = $k_subset;
        $count = count($set);
        $k = $count - 2;

        while ($k --) {
            $current = [];

            foreach ($k_subset as $item) {
                $c = $item['next'];

                while ($c < $count) {
                    $copy = $item['value'];
                    $copy[] = $set[$c];
                    $current[] = [
                        'next' => $c ++,
                        'value' => $copy
                    ];
                }
            }
            $k_subset = $current;
            $pre_subset = array_merge($pre_subset, $k_subset);
        }
        return array_map(function ($v) {
            return $v['value'];
        }, $pre_subset);
    }
    /*
    // 非递归获得真子集，二进制方式
    private function getProperSubsetUnrecursive($set) {
        $total = 1;
        $size = count($set);
        $subset = [];
        for ($n = 0; $n ++ < $size; $total <<= 1);
        for ($i = 1; $i < $total; $i ++) {
            $mask = $i;
            $subset[] = array_filter($set, function ($v) use (&$mask) {
                $flag = $mask & 1 == 1;
                $mask >>= 1;
                return $flag;
            });
        }
        return $subset;
    }*/
   
    /**
     * 获得关联规则
     * @param array $primary_data
     * @param float $support
     * @param float $confidence
     * @return number[][]|mixed[][]
     */
    public function getRelationRules(array $primary_data, $support, $confidence) {
        $data = [];
        foreach ($primary_data as $v) {
            $data[] = implode($this->_separation, $v);
        }
        $this->_data = $data;
        $this->_support = floatval($support);
        $this->_confidence = floatval($confidence);
        
        $relation = [];
        $frequents = $this->getFrequentCollections();
        $count = count($frequents);

        for ($i = 1; $i < $count; $i ++) {
            $frequent = $frequents[$i];

            foreach ($frequent as $freq_k => $freq_v) {
                $freq = explode($this->_separation, $freq_k);
                $subset = $this->getProperSubset($freq);

                foreach ($subset as $sub_item) {
                    $sub_item_str = implode($this->_separation, $sub_item);
                    $target_str = implode($this->_separation, array_diff($freq, $sub_item));
                    $prepare_str = implode($this->_connection, [$sub_item_str, $target_str]);

                    if (!isset($relation[$prepare_str])) {
                        $confidence_v = round($freq_v / $frequents[count($sub_item) - 1][$sub_item_str], 4);
                        $confidence_v >= $this->_confidence && $relation[$prepare_str] = [
                            'support' => $freq_v,
                            'confidence' => $confidence_v
                        ];
                    }
                }
            }
        }

        return $relation;
    }
}
