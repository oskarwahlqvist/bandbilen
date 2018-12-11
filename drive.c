#include "drive.h"

void forward()
{
    digitalWrite (leftBackward, 0);
    digitalWrite (rightBackward, 0);
    digitalWrite (leftForward, 1);
    digitalWrite (rightForward, 1);
}

void backward()
{
    digitalWrite (leftForward, 0);
    digitalWrite (rightForward, 0);
    digitalWrite (leftBackward, 1);
    digitalWrite (rightBackward, 1);
 }

void left()
{
    digitalWrite(rightBackward, 0);
    digitalWrite(leftForward, 0);
    digitalWrite(leftBackward, 1);
    digitalWrite(rightForward, 1);
}

void right()
{
    digitalWrite(leftBackward, 0);
    digitalWrite(rightForward, 0);
    digitalWrite(rightBackward, 1);
    digitalWrite(leftForward, 1);
}

void stopit()
{
    digitalWrite(leftForward, 0);
    digitalWrite(rightForward, 0);
    digitalWrite(leftBackward, 0);
    digitalWrite(rightBackward, 0);
}