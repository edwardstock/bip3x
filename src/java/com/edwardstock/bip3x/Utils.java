package com.edwardstock.bip3x;

import javax.annotation.Nonnull;
import javax.annotation.Nullable;

public class Utils {
    /**
     * Returns the first of two given parameters that is not {@code null}, if either is, or otherwise
     * throws a {@link NullPointerException}.
     *
     * <p>To find the first non-null element in an iterable, use {@code Iterables.find(iterable,
     * Predicates.notNull())}. For varargs, use {@code Iterables.find(Arrays.asList(a, b, c, ...),
     * Predicates.notNull())}, static importing as necessary.
     *
     * <p><b>Note:</b> if {@code first} is represented as an {@link Optional}, this can be
     * accomplished with {@link Optional#or(Object) first.or(second)}. That approach also allows for
     * lazy evaluation of the fallback instance, using {@link Optional#or(Supplier)
     * first.or(supplier)}.
     *
     * @return {@code first} if it is non-null; otherwise {@code second} if it is non-null
     * @throws NullPointerException if both {@code first} and {@code second} are null
     * @since 18.0 (since 3.0 as {@code Objects.firstNonNull()}).
     */
    public static <T> T firstNonNull(@Nullable T first, @Nullable T second) {
        if (first != null) {
            return first;
        }
        if (second != null) {
            return second;
        }
        throw new NullPointerException("Both parameters are null");
    }
    /**
     * Ensures that an object reference passed as a parameter to the calling
     * method is not null.
     *
     * @param reference an object reference
     * @return the non-null reference that was validated
     * @throws NullPointerException if {@code reference} is null
     */
    public static @Nonnull <T> T checkNotNull(@Nullable T reference) {
        if (reference == null) {
            throw new NullPointerException();
        }
        return reference;
    }

    /**
     * Ensures that an object reference passed as a parameter to the calling
     * method is not null.
     *
     * @param reference an object reference
     * @param errorMessage the exception message to use if the check fails; will
     *     be converted to a string using {@link String#valueOf(Object)}
     * @return the non-null reference that was validated
     * @throws NullPointerException if {@code reference} is null
     */
    public static @Nonnull <T> T checkNotNull(@Nullable T reference, @Nonnull Object errorMessage) {
        if (reference == null) {
            throw new NullPointerException(String.valueOf(errorMessage));
        }
        return reference;
    }
}
