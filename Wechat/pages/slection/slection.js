// pages/slection/slection.js
Page({
  data: {

  },
  tel: function () {
    wx.makePhoneCall({
      phoneNumber: '120',
    })
  },
  tel2: function () {
    wx.makePhoneCall({
      phoneNumber: '15600338263',
    })
  }

})