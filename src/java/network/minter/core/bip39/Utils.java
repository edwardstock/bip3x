package network.minter.core.bip39;

import javax.annotation.Nonnull;

public class Utils {

    /**
     * Ensures that an object reference passed as a parameter to the calling
     * method is not null.
     *
     * @param reference an object reference
     * @return the non-null reference that was validated
     * @throws NullPointerException if {@code reference} is null
     */
    @Nonnull
    public static <T> T checkNotNull(final T reference, final String message) {
        if (reference == null) {
            throw new NullPointerException(message);
        }
        return reference;
    }

    @Nonnull
    public static <T> T checkNotNull(final T reference) {
        return checkNotNull(reference, null);
    }

    @Nonnull
    public static <T> T firstNonNull(final T ref0, final T... refs) {
        if (ref0 != null) {
            return ref0;
        }

        T outRef = null;
        for (T ref : refs) {
            if (ref != null) {
                outRef = ref;
                break;
            }
        }

        return checkNotNull(outRef);
    }
}
