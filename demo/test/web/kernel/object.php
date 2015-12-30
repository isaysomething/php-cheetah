<?php
// Get class name
echo Cheetah\Kernel\Object::className();
echo '<br>';

// Invoke init function
class A extends Cheetah\Kernel\Object
{
    public $x;
    public $y;

    public function init()
    {
        echo 'A:init()';
    }

    public function setA($value)
    {
        echo 'sss';
    }

    public function getC()
    {
        return 'aaa';
    }
}

$a = new A([
    'x' => 'x',
    'y' => ['aa'],
    'aa'
]);
var_dump($a);
//$a = new A();

// getter function
//echo $a->a;
//echo $a->b;
//$a->c = 'c';
//$a->d = 'd';