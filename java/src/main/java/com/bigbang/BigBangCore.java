package com.bigbang;

import com.bigbang.exception.CreateTransactionException;
import com.bigbang.exception.SendTransactionException;
import com.bigbang.pojo.CreateTransactionParams;
import com.bigbang.pojo.Error;
import com.bigbang.pojo.RPCPayload;
import com.bigbang.pojo.SendTransactionParams;
import com.fasterxml.jackson.core.type.TypeReference;
import com.fasterxml.jackson.databind.ObjectMapper;
import okhttp3.*;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.IOException;

public class BigBangCore {

    private static final Logger logger = LoggerFactory.getLogger(BigBangCore.class);

    public static final MediaType JSON = MediaType.parse("application/json; charset=utf-8");

    public static final ObjectMapper MAPPER = new ObjectMapper();

    private String rpcUrl;

    private OkHttpClient httpClient;

    private BigBangCore() {

    }

    public static Builder builder() {
        return new Builder();
    }

    private void init() {
        httpClient = new OkHttpClient.Builder().build();
    }

    public static class Builder {

        private static final String DEFAULT_RPC_URL = "http://127.0.0.1:9902";

        private String rpcUrl = DEFAULT_RPC_URL;

        public Builder rpcUrl(String url) {
            this.rpcUrl = url;
            return this;
        }

        public BigBangCore build() {
            BigBangCore bigBangCore = new BigBangCore();
            bigBangCore.rpcUrl = this.rpcUrl;
            bigBangCore.init();
            return bigBangCore;
        }
    }

    static {
        try {
            logger.info("java.library.path={}", System.getProperty("java.library.path"));
            System.loadLibrary("offlinesign");
        } catch (Exception e) {
            logger.error("Failed to load library offlinesign.", e);
        }
    }

    /**
     * Generate public key and private key
     *
     * @return The key pair array, result[0] is public key, result[1] is private key
     */
    public native String[] makeKeyPair();

    /**
     * Get Address hex string
     *
     * @param publicKey public key hex string
     * @return Address hex string
     */
    public native String getPubKeyAddress(String publicKey);

    /**
     * Validate Address hex string
     *
     * @param address address hex string
     * @return valid
     */
    public native boolean isValidAddress(String address);

    /**
     * Sign the given transaction
     *
     * @param tx        transaction
     * @param secretHex secretKey hex string
     */
    public native void signTransaction(Transaction tx, String secretHex);

    /**
     * Serialize the transaction
     *
     * @param tx
     * @return The data hex string of serialized transaction
     */
    public native String serializeTxWithoutSign(Transaction tx);

    /**
     * Serialize the transaction include sign
     *
     * @param tx
     * @return The data hex string of serialized transaction
     */
    public native String serializeTxWithSign(Transaction tx);

    /**
     * Deserialize a transaction with sign from the hex string
     *
     * @param hexString hex string which need to be deserialized
     * @return transaction
     */
    public native Transaction deserializeTxWithSign(String hexString);

    /**
     * Deserialize a transaction from the hex string
     *
     * @param hexString hex string which need to be deserialized
     * @return transaction
     */
    public native Transaction deserializeTxWithoutSign(String hexString);

    public String createTransaction(CreateTransactionParams params) throws IOException, CreateTransactionException {
        RPCPayload<CreateTransactionParams> rpcParams = new RPCPayload<>(59, "createtransaction", params);
        String json = MAPPER.writeValueAsString(rpcParams);
        RequestBody body = RequestBody.create(JSON, json);
        logger.debug("\npost url: {}\nbody: {}", rpcUrl, json);
        Request request = new Request.Builder()
                .url(rpcUrl)
                .post(body)
                .build();
        Response response = httpClient.newCall(request).execute();
        ResponseBody responseBody = response.body();
        if (responseBody == null) {
            throw new IllegalStateException("The core wallet is in illegal state cause creating transaction is failed.");
        }
        RPCPayload<String> rpcPayload = MAPPER.readValue(responseBody.bytes(), new TypeReference<RPCPayload<String>>() {
        });
        Error error = rpcPayload.getError();
        if (error != null) {
            throw new CreateTransactionException(error.getMessage(), error.getCode());
        }
        return rpcPayload.getResult();

    }

    public String sendTransaction(SendTransactionParams params) throws IOException, SendTransactionException {
        RPCPayload<SendTransactionParams> rpcParams = new RPCPayload<>(9, "sendtransaction", params);
        String json = MAPPER.writeValueAsString(rpcParams);
        RequestBody body = RequestBody.create(JSON, json);
        logger.debug("\npost url: {}\nbody: {}", rpcUrl, json);
        Request request = new Request.Builder()
                .url(rpcUrl)
                .post(body)
                .build();
        Response response = httpClient.newCall(request).execute();
        ResponseBody responseBody = response.body();
        if (responseBody == null) {
            throw new IllegalStateException("The core wallet is in illegal state cause sending transaction is failed.");
        }
        RPCPayload<String> rpcPayload = MAPPER.readValue(responseBody.bytes(), new TypeReference<RPCPayload<String>>() {
        });
        Error error = rpcPayload.getError();
        if (error != null) {
            throw new SendTransactionException(error.getMessage(), error.getCode());
        }
        return rpcPayload.getResult();
    }

}
