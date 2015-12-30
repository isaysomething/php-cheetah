<?php
/**
 * Created by PhpStorm.
 * User: yrssoft
 * Date: 15-12-22
 * Time: 下午8:09
 */

namespace Cheetah\Kernel;


class Exception extends \Exception
{

    public function getName()
    {
        return "Exception";
    }

}