#!/bin/sh

TOPDIR=${TOPDIR:-$(git rev-parse --show-toplevel)}
SRCDIR=${SRCDIR:-$TOPDIR/src}
MANDIR=${MANDIR:-$TOPDIR/doc/man}

PLACEHD=${PLACEHD:-$SRCDIR/placehd}
PLACEHCLI=${PLACEHCLI:-$SRCDIR/placeh-cli}
PLACEHTX=${PLACEHTX:-$SRCDIR/placeh-tx}
PLACEHQT=${PLACEHQT:-$SRCDIR/qt/placeh-qt}

[ ! -x $PLACEHD ] && echo "$PLACEHD not found or not executable." && exit 1

# The autodetected version git tag can screw up manpage output a little bit
RVNVER=($($PLACEHCLI --version | head -n1 | awk -F'[ -]' '{ print $6, $7 }'))

# Create a footer file with copyright content.
# This gets autodetected fine for placehd if --version-string is not set,
# but has different outcomes for placeh-qt and placeh-cli.
echo "[COPYRIGHT]" > footer.h2m
$PLACEHD --version | sed -n '1!p' >> footer.h2m

for cmd in $PLACEHD $PLACEHCLI $PLACEHTX $PLACEHQT; do
  cmdname="${cmd##*/}"
  help2man -N --version-string=${RVNVER[0]} --include=footer.h2m -o ${MANDIR}/${cmdname}.1 ${cmd}
  sed -i "s/\\\-${RVNVER[1]}//g" ${MANDIR}/${cmdname}.1
done

rm -f footer.h2m
