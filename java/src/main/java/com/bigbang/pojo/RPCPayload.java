package com.bigbang.pojo;

import com.fasterxml.jackson.annotation.JsonInclude;
import com.fasterxml.jackson.annotation.JsonProperty;

@JsonInclude(JsonInclude.Include.NON_NULL)
public class RPCPayload<T> {

    private int id;

    @JsonProperty("jsonrpc")
    private String jsonRpc = "2.0";

    private String method;

    private T params;

    private T result;

    private Error error;

    public RPCPayload(int id, String method, T params) {
        this.id = id;
        this.method = method;
        this.params = params;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getJsonRpc() {
        return jsonRpc;
    }

    public String getMethod() {
        return method;
    }

    public void setMethod(String method) {
        this.method = method;
    }

    public T getParams() {
        return params;
    }

    public void setParams(T params) {
        this.params = params;
    }

    public T getResult() {
        return result;
    }

    public void setResult(T result) {
        this.result = result;
    }

    public Error getError() {
        return error;
    }

    public void setError(Error error) {
        this.error = error;
    }

    public RPCPayload() {
    }
}
