#! /bin/bash

VERSION=$(<VERSION)
PROJECT=paste-binouse
DISTDIR=$PROJECT-$VERSION

sed -i "s/PROJECT_NUMBER.*/PROJECT_NUMBER=$VERSION/g" Doxyfile
rm -rf $DISTDIR
mkdir $DISTDIR

cp -R \
    AUTHORS \
    CMakeLists.txt \
    ChangeLog \
    dist.sh \
    Doxyfile \
    LICENSE \
    README \
    scripts \
    sql \
    src \
    VERSION \
    www \
    $DISTDIR

tar -cvvv $DISTDIR | xz -ze9c >$DISTDIR.tar.xz
rm -rf $DISTDIR

md5sum $DISTDIR.tar.xz >$DISTDIR.tar.xz.md5sum
