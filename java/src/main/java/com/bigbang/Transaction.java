package com.bigbang;

public class Transaction {
    private int version;
    private int type;
    private long timestamp;
    private long lockUntil;
    private String hashAnchor;
    private byte[] input;
    private short prefix;
    private String address;
    private long amount;
    private long txFee;
    private byte[] vchData;
    private String sign;

    public int getVersion() {
        return version;
    }

    public void setVersion(int version) {
        this.version = version;
    }

    public int getType() {
        return type;
    }

    public void setType(int type) {
        this.type = type;
    }

    public long getTimestamp() {
        return timestamp;
    }

    public void setTimestamp(long timestamp) {
        this.timestamp = timestamp;
    }

    public long getLockUntil() {
        return lockUntil;
    }

    public void setLockUntil(long lockUntil) {
        this.lockUntil = lockUntil;
    }

    public String getHashAnchor() {
        return hashAnchor;
    }

    public void setHashAnchor(String hashAnchor) {
        this.hashAnchor = hashAnchor;
    }


    public byte[] getInput() {
        return input;
    }

    public void setInput(byte[] input) {
        this.input = input;
    }

    public short getPrefix() {
        return prefix;
    }

    public void setPrefix(short prefix) {
        this.prefix = prefix;
    }

    public String getAddress() {
        return address;
    }

    public void setAddress(String address) {
        this.address = address;
    }

    public long getAmount() {
        return amount;
    }

    public void setAmount(long amount) {
        this.amount = amount;
    }

    public long getTxFee() {
        return txFee;
    }

    public void setTxFee(long txFee) {
        this.txFee = txFee;
    }

    public byte[] getVchData() {
        return vchData;
    }

    public void setVchData(byte[] vchData) {
        this.vchData = vchData;
    }

    public String getSign() {
        return sign;
    }

    public void setSign(String sign) {
        this.sign = sign;
    }
}
