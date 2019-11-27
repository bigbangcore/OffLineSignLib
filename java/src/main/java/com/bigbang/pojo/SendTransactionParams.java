package com.bigbang.pojo;

import com.fasterxml.jackson.annotation.JsonInclude;
import com.fasterxml.jackson.annotation.JsonProperty;

@JsonInclude(JsonInclude.Include.NON_NULL)
public class SendTransactionParams {

    @JsonProperty("txdata")
    private String txData;

    public String getTxData() {
        return txData;
    }

    public void setTxData(String txData) {
        this.txData = txData;
    }
}
