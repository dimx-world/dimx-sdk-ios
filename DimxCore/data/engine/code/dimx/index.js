import {Dimension} from 'dimx-cpp'
import {ContentFactory} from './lib/ContentFactory'

if (!Dimension.contentFactory) {
    Dimension.contentFactory = new ContentFactory();
    Dimension.on('AddDummy', Dimension.contentFactory.createContent.bind(Dimension.contentFactory));
}

export * from 'dimx-cpp'
