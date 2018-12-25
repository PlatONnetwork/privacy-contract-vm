# this, only for java
# default directory/package is mpc

PACKAGE_BEG
/*
 * may be some declaration here.
 */
package platon.mpc.proxy;

import com.google.protobuf.InvalidProtocolBufferException;
import net.platon.mpc.proxy.sdk.*;
import org.web3j.crypto.Credentials;
import org.web3j.protocol.Web3j;
import org.web3j.utils.Numeric;

import java.util.HashMap;

PACKAGE_END

COMMENT_BEG
/**
 * Attention! This file was auto-generated, DO NOT EDIT!
 * More details ref "IRNAME-README.TXT".
 * <p>
 * DIGEST:
 * <p>
IRDIGEST */
COMMENT_END

PROXY_CLASS_BEG
public class IRNAME extends ProxyClient {
    private String irHash = "IRHASH";
    private static String sIrHash;

    /**
     * Enum Method
     */
    public enum Method {
IR_FUNC_PROT
    }

    private static HashMap<Method, MethodInfo> mapMethodInfo = new HashMap<Method, MethodInfo>() {{METHOD_MAP
    }};

    public static void showMethodMap() {
        System.out.println("==== Method Map Beg ====");
        for (MethodInfo info : mapMethodInfo.values()) {
            System.out.println(info.name + "    " + info.prot + "    " + info.enum_name);
        }
        System.out.println("==== Method Map End ====");
    }
    
    /**
     * Constructor
     */
    public IRNAME(Web3j web3j, Credentials credentials) {
        super(web3j, credentials);
    }

    public IRNAME(String url, Credentials credentials) {
        super(url, credentials);
    }

    public IRNAME(String url, String walletPath, String walletPass) {
        super(url, walletPath, walletPass);
    }

    public IRNAME(Web3j web3j, String walletPath, String walletPass) {
        super(web3j, walletPath, walletPass);
    }

    /**
     * Public method
     */
    public String startCalc(Method method) {
        return startCalc(method, 0);
    }

    public String startCalc(Method method, int retry) {
        if (!mapMethodInfo.containsKey(method)) {
            // no this method
            return null;
        }
        return startCalc(mapMethodInfo.get(method).name, retry);
    }

    /*
     * Below getXXX, if have, only for this mpc contract
     */
     PROXY_PROTO_RETURN_FUNC_S
}
PROXY_CLASS_END

PROXY_PROTO_RETURN_FUNC_BEG
    /**
     * Get RETPKG.RETTYPE
     */
    public RETPKG.RETTYPE getRETTYPE(byte[] val) {
        RETPKG.RETTYPE __RETTYPE = null;
        try {
            __RETTYPE = RETPKG.RETTYPE.parseFrom(ethECIES.Decrypt(priHexString, val));
        } catch (InvalidProtocolBufferException e) {
            e.printStackTrace();
        }
        return __RETTYPE;
    }

    public RETPKG.RETTYPE getRETTYPE(String cipher) {
        return getRETTYPE(Numeric.hexStringToByteArray(cipher));
    }
PROXY_PROTO_RETURN_FUNC_END

METHOD_MAP_BEG
        put(Method.IR_FUNC_PROT, new MethodInfo("IRFUNCNAME", "IRFUNCPROTO", "IRFUNCHASH", "", "IR_FUNC_PROT"));
METHOD_MAP_END

README_BEG
THIS FILE IS AUTO-GENERATED! DO NOT EDIT!

DIGEST:
/**
IRDIGEST */

README_END
