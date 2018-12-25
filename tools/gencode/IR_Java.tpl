# this, only for java
# default directory/package is mpc

PACKAGE_BEG
/*
 * may be some declaration here.
 */
package net.platon.vm.mpc;

import net.platon.vm.sdk.client.Data;
import net.platon.vm.sdk.client.ErrorCode;
import net.platon.vm.sdk.client.IIInterface;
import net.platon.vm.sdk.client.InputRequestPara;
import net.platon.vm.sdk.client.MpcCallbackInterface;

import java.util.HashMap;

PACKAGE_END

COMMENT_BEG
/**
 * Attention! This file was auto-generated, you just need to implement the "TODO SECTIONS".
 * The class name "IRNAME" is just to named this file, you can rename "IRNAME" what you like.
 * More details ref "IRNAME-README.TXT".
 * <p>
 * DIGEST:
 * <p>
IRDIGEST */
 COMMENT_END

IR_FUNC_INTERFACE_BEG
        put("mpc_f_IRFUNCHASH_IRFUNCPOSX", new IRNAME_IRFUNCPROTA_IRFUNCPOSX());
IR_FUNC_INTERFACE_END

ABSTRACT_BASE_BEG
interface mpc_ii_IRHASH extends IIInterface {
}

public class IRNAME implements mpc_ii_IRHASH {
    private HashMap<String, MpcCallbackInterface> funcInterfaces = new HashMap<String, MpcCallbackInterface>() {{IR_FUNC_INTERFACES    }};

    public MpcCallbackInterface getInstance(String instance_hash) {
        if (funcInterfaces.containsKey(instance_hash)) {
            return funcInterfaces.get(instance_hash);
        }
        return null;
    }

    public HashMap<String, MpcCallbackInterface> getInstances() {
        return funcInterfaces;
    }

    abstract class MpcCallbackBase_IRHASH implements MpcCallbackInterface {
        public abstract byte[] inputImpl(final InputRequestPara para);
        
        public byte[] input(final InputRequestPara para) {
            // TODO: do what you want to do, before call inputImpl
            return inputImpl(para);
        }

        public void error(final InputRequestPara para, ErrorCode error) {
            // TODO: do what you want to do
        }

        public void result(final InputRequestPara para, final byte[] data) {
            // TODO: do what you want to do
        }
    }

    abstract class mpc_i_IRHASH extends MpcCallbackBase_IRHASH {
    }
    CONTENT_FUNC_ABSTRACT
    CONTENT_FUNC_EXTENDS
}
ABSTRACT_BASE_END

FUNC_ABSTRACT_BEG
    abstract class mpc_f_IRFUNCHASH_IRFUNCPOSX extends mpc_i_IRHASH {
        public byte[] input_x_default(){
            return X_DEFAULT_INPUT;
        }
    }
FUNC_ABSTRACT_END

FUNC_EXTENDS_BEG
    /**
     * IRFUNCPROT
     */
    final class IRNAME_IRFUNCPROTA_IRFUNCPOSX extends mpc_f_IRFUNCHASH_IRFUNCPOSX {
        public byte[] inputImpl(final InputRequestPara para) {
            RETTYPE_DATADEFAULTVALUE;
            // TODO: assemble data
            
            return RETVALUEFUNC;
        }
    }
FUNC_EXTENDS_END


README_BEG
/*
 * may be some declaration here.
 */
THIS FILE IS AUTO-GENERATED!

DIGEST:
/**
IRDIGEST */

IN GENERAL, YOU JUST NEED TO IMPLEMENT THE "TODO SECTION" IN IRNAME.java.

You can rename "IRNAME.java" to any you like, but do not change the md5-string in that .java file.

When you implement "inputImpl", you can do some work before, log or store the request info,
in the MpcCallbackBasexx.

todo......

README_END
