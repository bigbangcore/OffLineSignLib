package com.bigbang.exception;

public class BigBangException extends Exception {
    private int code;

    public BigBangException(String message, int code) {
        super(message);
        this.code = code;
    }
}
