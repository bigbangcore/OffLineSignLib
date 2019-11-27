package com.bigbang.exception;

public class SendTransactionException extends BigBangException {
    public SendTransactionException(String message, int code) {
        super(message, code);
    }
}
