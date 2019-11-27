package com.bigbang.pojo;

import com.fasterxml.jackson.annotation.JsonInclude;
import com.fasterxml.jackson.annotation.JsonProperty;

@JsonInclude(JsonInclude.Include.NON_NULL)
public class CreateTransactionParams {

    @JsonProperty("from")
    private String fromAddress;

    @JsonProperty("to")
    private String toAddress;

    private double amount;

    @JsonProperty("txfee")
    private Double txFee;

    @JsonProperty("fork")
    private String forkHash;

    private String data;

    public String getFromAddress() {
        return fromAddress;
    }

    public void setFromAddress(String fromAddress) {
        this.fromAddress = fromAddress;
    }

    public String getToAddress() {
        return toAddress;
    }

    public void setToAddress(String toAddress) {
        this.toAddress = toAddress;
    }

    public double getAmount() {
        return amount;
    }

    public void setAmount(double amount) {
        this.amount = amount;
    }

    public Double getTxFee() {
        return txFee;
    }

    public void setTxFee(Double txFee) {
        this.txFee = txFee;
    }

    public String getForkHash() {
        return forkHash;
    }

    public void setForkHash(String forkHash) {
        this.forkHash = forkHash;
    }

    public String getData() {
        return data;
    }

    public void setData(String data) {
        this.data = data;
    }

    @Override
    public String toString() {
        return "CreateTransactionParams{" +
                "fromAddress=" + fromAddress +
                ", toAddress=" + toAddress +
                ", amount=" + amount +
                ", txFee=" + txFee +
                ", forkHash=" + forkHash +
                ", data=" + data +
                '}';
    }
}
